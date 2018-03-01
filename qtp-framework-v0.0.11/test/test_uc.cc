//
// Created by wen on 2016-04-15.
//

#include "universal_code.h"
#include <iostream>

using namespace qtp;

int main() {

  uc_t uc = UniversalCode::SymbolToUC("000927", kMC_SSE, kSC_EQUITY);
  std::cout << uc << std::endl;

  std::string symbol;
  MarketCode mc = kMC_UNKNOW;
  SecurityCategory sc = kSC_UNKNOW;
  UniversalCode::UCToSymbol(uc, &symbol, &mc, &sc);
  std::cout << symbol.size() << std::endl;
  std::cout << "symbol: " << symbol << " market: " << mc << " category: " << sc << std::endl;

  return 0;
}