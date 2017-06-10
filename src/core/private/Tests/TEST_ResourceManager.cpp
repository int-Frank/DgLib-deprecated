#include "TestHarness.h"
#include "ResourceManager.h"

typedef Dg::Rm::RKey rKey;

class TestResource : public Dg::Rm::Resource
{
public:

  TestResource(Dg::Rm::RKey a_key) : Resource(a_key, nullptr), isInit(false) {}
  bool IsInitialised() { return isInit; }

  Dg::ErrorCode Init() { isInit = true; return Dg::Err_None; }
  Dg::ErrorCode DeInit() { isInit = false; return Dg::Err_None; }

private:
  bool isInit;
};


TEST(Stack_ResourceKey, creation_ResourceKey)
{
  rKey key0 = 0xF0;
  rKey key1 = 0xF1;
  rKey key2 = 0xF2;
  rKey key3 = 0xF3;

  Dg::ErrorCode result;
  Dg::Rm::SetOptions(Dg::Rm::rmDEFAULT);

  CHECK(Dg::Rm::RegisterResource(new TestResource(key0)) == Dg::Err_None);
  
  Dg::Rm::hResource handle;
  CHECK(Dg::Rm::GetResourceHandle(key1, handle) == Dg::Err_Failure);

  result = Dg::Rm::GetResourceHandle(key0, handle);
  CHECK(result == Dg::Err_None);
  
  if (result == Dg::Err_None)
  {
    CHECK(handle->IsInitialised());

    Dg::Rm::hResource handle2(handle);
    CHECK(handle2->IsInitialised());

    Dg::Rm::hResource handle3 = handle;
    CHECK(handle3->IsInitialised());
  }

}