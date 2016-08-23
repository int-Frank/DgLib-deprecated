#include <iostream>
#include <fstream>
#include <iomanip>

#include "TG_erf.h"
#include "TG_inverf.h"
#include "TG_HashTableBucketCounts.h"
#include "TG_n_pow_i.h"

int main()
{
  TG_n_pow_i<uint64_t> tg(16, 0xFFFFFFFFFFFFFFFF, sqrt(2.0));
  tg.Generate();
  tg.Output(std::hex, 8, 0);
}