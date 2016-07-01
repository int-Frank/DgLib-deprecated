
#define ADD_SINGLE_MEMBER(NAME, TYPE) TYPE * m_ ## NAME;
#define ADD_MEMBERS(...) GLUE(ADD_ITEM_HELPER(NARGS(__VA_ARGS__)),(MEMBER, __VA_ARGS__))

#define ADD_SINGLE_METHOD(NAME, TYPE) TYPE * Get ## NAME() {return m_ ## NAME;}
#define ADD_METHODS(...) GLUE(ADD_ITEM_HELPER(NARGS(__VA_ARGS__)),(METHOD, __VA_ARGS__))

#define ADD_SINGLE_ENUM(NAME, TYPE) NAME,
#define ADD_ENUM_ENTRIES(...) GLUE(ADD_ITEM_HELPER(NARGS(__VA_ARGS__)),(ENUM, __VA_ARGS__))

#define ADD_SINGLE_CONSTRUCTOR(NAME, TYPE) m_ ## NAME(nullptr),
#define ADD_MEMBER_CONSTRUCTORS(...) GLUE(ADD_ITEM_HELPER(NARGS(__VA_ARGS__)),(CONSTRUCTOR, __VA_ARGS__))


#define ADD_SINGLE_DESTRUCTOR(NAME, TYPE) delete[] m_ ## NAME;
#define ADD_MEMBER_DESTRUCTORS(...) GLUE(ADD_ITEM_HELPER(NARGS(__VA_ARGS__)),(DESTRUCTOR, __VA_ARGS__))

#define ADD_SINGLE_KILL(NAME, TYPE) \
if (m_ ## NAME)\
{\
  m_ ## NAME[a_index] = m_ ## NAME[m_countAlive];\
}
#define ADD_KILL_CODE(...) GLUE(ADD_ITEM_HELPER(NARGS(__VA_ARGS__)),(KILL, __VA_ARGS__))

#define ADD_SINGLE_INIT(NAME, TYPE) \
case ParticleAttr::NAME:\
{\
  if (m_ ## NAME == nullptr) {m_ ## NAME = new TYPE[m_countMax];}\
  break;\
}
#define ADD_INIT_CODE(...) GLUE(ADD_ITEM_HELPER(NARGS(__VA_ARGS__)),(INIT, __VA_ARGS__))

#define ADD_SINGLE_DEINIT(NAME, TYPE) \
case ParticleAttr::NAME:\
{\
  delete[] m_ ## NAME;\
  m_ ## NAME = nullptr;\
  break;\
}
#define ADD_DEINIT_CODE(...) GLUE(ADD_ITEM_HELPER(NARGS(__VA_ARGS__)),(DEINIT, __VA_ARGS__))