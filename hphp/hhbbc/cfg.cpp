/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-2013 Facebook, Inc. (http://www.facebook.com)     |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/
#include "hphp/hhbbc/cfg.h"

#include <boost/dynamic_bitset.hpp>

namespace HPHP { namespace HHBBC {

//////////////////////////////////////////////////////////////////////

namespace {

void postorderWalk(std::vector<borrowed_ptr<php::Block>>& out,
                   boost::dynamic_bitset<>& visited,
                   php::Block& blk) {
  if (visited[blk.id]) return;
  visited[blk.id] = true;
  forEachSuccessor(blk, [&] (php::Block& next) {
    postorderWalk(out, visited, next);
  });
  out.push_back(&blk);
}

}

//////////////////////////////////////////////////////////////////////

std::vector<borrowed_ptr<php::Block>> rpoSortFromMain(const php::Func& func) {
  boost::dynamic_bitset<> visited(func.nextBlockId);
  std::vector<borrowed_ptr<php::Block>> ret;
  ret.reserve(func.nextBlockId);
  postorderWalk(ret, visited, *func.mainEntry);
  std::reverse(begin(ret), end(ret));
  return ret;
}

std::vector<borrowed_ptr<php::Block>> rpoSortAddDVs(const php::Func& func) {
  boost::dynamic_bitset<> visited(func.nextBlockId);
  std::vector<borrowed_ptr<php::Block>> ret;
  ret.reserve(func.nextBlockId);
  postorderWalk(ret, visited, *func.mainEntry);

  /*
   * Note: depending what the caller is planning to do with this, it
   * is easy to rely on the language in the bytecode spec which says
   * that DV entry points must be basic blocks.
   *
   * E.g. if you use rpoSortAddDVs to see which blocks are reachable,
   * you can leave out blocks within a given DV entry if we support
   * multi-block DV entries.
   */
  for (auto rit = func.params.rbegin(); rit != func.params.rend(); ++rit) {
    if (rit->dvEntryPoint) ret.push_back(rit->dvEntryPoint);
  }
  std::reverse(begin(ret), end(ret));
  return ret;
}

//////////////////////////////////////////////////////////////////////

}}
