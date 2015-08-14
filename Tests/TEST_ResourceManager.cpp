#include "TestHarness.h"
#include "ResourceManager.h"

typedef Dg::Rm::RKey rKey;

class TestResource : public Dg::Rm::Resource
{
public:

  TestResource(Dg::Rm::RKey a_key) : Resource(a_key), isInit(false) {}
  bool IsInitialised() { return isInit; }

  Dg_Result Init() { isInit = true; return DgR_Success; }
  Dg_Result DeInit() { isInit = false; return DgR_Success; }

private:
  bool isInit;
};


TEST(Stack_ResourceKey, creation_ResourceKey)
{
  rKey key0 = 0xF0;
  rKey key1 = 0xF1;
  rKey key2 = 0xF2;
  rKey key3 = 0xF3;

  Dg_Result result;
  Dg::Rm::SetOptions(Dg::Rm::rmDEFAULT);

  CHECK(Dg::Rm::RegisterResource(new TestResource(key0)) == DgR_Success);
  
  Dg::Rm::hResource handle;
  CHECK(Dg::Rm::GetResourceHandle(key1, handle) == DgR_Failure);

  result = Dg::Rm::GetResourceHandle(key0, handle);
  CHECK(result == DgR_Success);
  
  if (result == DgR_Success)
  {
    CHECK(handle->IsInitialised());

    Dg::Rm::hResource handle2(handle);
    CHECK(handle2->IsInitialised());

    Dg::Rm::hResource handle3 = handle;
    CHECK(handle3->IsInitialised());
  }

}