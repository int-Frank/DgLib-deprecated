#include <iostream>
#include <fstream>
#include <iomanip>

#include "TG_erf.h"
#include "TG_inverf.h"
#include "TG_HashTableBucketCounts.h"

int main()
{
  TG_HashTableBucketCounts<uint64_t> tg(4, 0xFFFFFFFF, sqrt(2.0));
  tg.Generate();
  tg.Output(std::hex, 8, 0);
}