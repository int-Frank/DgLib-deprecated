
#define ADD_SINGLE_MEMBER(type, name) type * m_ ## name;
#define ADD_MEMBERS(...) GLUE(ADD_ITEM_HELPER(NARGS(__VA_ARGS__)),(MEMBER, __VA_ARGS__))

#define ADD_SINGLE_METHOD(t0, n0) void Init ## n0() { if (m_ ## n0 == nullptr) {m_ ## n0 = new t0[m_maxCount];} }\
                             void Deinit ## n0() { delete[] m_ ## n0; m_ ## n0 = nullptr;}\
                             t0 * Get ## n0() {return m_ ## n0;}
#define ADD_METHODS(...) GLUE(ADD_ITEM_HELPER(NARGS(__VA_ARGS__)),(METHOD, __VA_ARGS__))

#define ADD_SINGLE_ENUM(t0, n0) n0,
#define ADD_ENUM_ENTRIES(...) GLUE(ADD_ITEM_HELPER(NARGS(__VA_ARGS__)),(ENUM, __VA_ARGS__))

#define ADD_SINGLE_CONSTRUCTOR(t0, n0) m_ ## n0(nullptr),
#define ADD_MEMBER_CONSTRUCTORS(...) GLUE(ADD_ITEM_HELPER(NARGS(__VA_ARGS__)),(CONSTRUCTOR, __VA_ARGS__))


#define ADD_SINGLE_DESTRUCTOR(t0, n0) delete[] m_ ## n0;
#define ADD_MEMBER_DESTRUCTORS(...) GLUE(ADD_ITEM_HELPER(NARGS(__VA_ARGS__)),(DESTRUCTOR, __VA_ARGS__))

#define ADD_SINGLE_WAKE(t0, n0) \
if (SavedAttr & (1 << n0))\
{\
  if (m_ ## n0)\
  {\
    int temp(m_ ## n0[m_countAlive]);\
    m_ ## n0[m_countAlive] = m_ ## n0[m_countMax - 1];\
    m_ ## n0[m_countMax - 1] = temp;\
  }\
} 
#define ADD_WAKE_CODE(...) GLUE(ADD_ITEM_HELPER(NARGS(__VA_ARGS__)),(WAKE, __VA_ARGS__))

#define ADD_SINGLE_KILL \
if (m_ ## n0)\
{\
  if (SavedAttr & (1 <<  ## n0))\
  {\
    int temp(m_ ## n0[a_index]);\
    m_ ## n0[a_index] = m_ ## n0[m_countAlive];\
    m_ ## n0[m_countAlive] = temp;\
  }\
  else\
  {\
    m_ ## n0[a_index] = m_ ## n0[m_countAlive];\
  }\
}
#define ADD_KILL_CODE(...) GLUE(ADD_ITEM_HELPER(NARGS(__VA_ARGS__)),(KILL, __VA_ARGS__))
