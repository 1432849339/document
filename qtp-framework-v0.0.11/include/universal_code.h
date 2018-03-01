//
// Created by wen on 2016-04-15.
//

#ifndef QTP_FRAMEWORK_UNIVERSAL_CODE_H
#define QTP_FRAMEWORK_UNIVERSAL_CODE_H

#include <stdint.h>
#include <string>

namespace qtp {

//4bit for security catetory, 12bit for market code, 48bit for symbol code
typedef uint64_t uc_t;

const uc_t INVALID_UC = 0;

enum SecurityCategory {
  kSC_UNKNOW = 0,
  kSC_EQUITY,
  kSC_FUTURE,
};

enum MarketCode {
  kMC_UNKNOW = 0,
  kMC_SSE,
  kMC_SZE,
  kMC_CFFE,
};

class UniversalCode {
public:
  /**
   * @desc convert symbol character to universal code
   * @param symbol symbol of a security
   * @param mc market code of symbol
   * @param security category of symbol
   * @return universal code of symbol or INVALID_UC for error
   */
  static uc_t SymbolToUC(const std::string& symbol, MarketCode mc = kMC_UNKNOW, SecurityCategory sc = kSC_UNKNOW);

  /**
   * @desc convert universal code to symbol character
   * input:
   * @param uc universal code of a security
   * output:
   * @param symbol symbol of uc, pass nullptr if you not care
   * @param mc market code of uc, pass nullptr if you not care
   * @param security category of uc, pass nullptr if you not care
   * @return 0 for success or 1 for error
   */
  static int UCToSymbol(uc_t uc, std::string* symbol, MarketCode* mc, SecurityCategory* sc);

  static const size_t kSC_BITS = 4;
  static const size_t kMC_BITS = 12;
  static const size_t kCODE_BITS = 48;
  static const uint64_t kSC_MASK = (1ULL << kSC_BITS) - 1;
  static const uint64_t kMC_MASK = (1ULL << kMC_BITS) - 1;
  static const uint64_t kCODE_MASK = (1ULL << kCODE_BITS) - 1;
  static const size_t kSC_SHIFT_BITS = kCODE_BITS + kMC_BITS;
  static const size_t kMC_SHIFT_BITS = kCODE_BITS;
};

} //namespace qtp

#endif //QTP_FRAMEWORK_UNIVERSAL_CODE_H
