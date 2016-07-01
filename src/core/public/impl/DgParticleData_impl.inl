
#define ADD_SINGLE_MEMBER(_name, _type) _type * m_ ## _name;
#define ADD_MEMBERS(...) GLUE(ADD_ITEM_HELPER(NARGS(__VA_ARGS__)),(MEMBER, __VA_ARGS__))

#define ADD_SINGLE_METHOD(_name, _type) void Init ## _name() { if (m_ ## _name == nullptr) {m_ ## _name = new _type[m_maxCount];} }\
                             void Deinit ## _name() { delete[] m_ ## _name; m_ ## _name = nullptr;}\
                             _type * Get ## _name() {return m_ ## _name;}
#define ADD_METHODS(...) GLUE(ADD_ITEM_HELPER(NARGS(__VA_ARGS__)),(METHOD, __VA_ARGS__))

#define ADD_SINGLE_ENUM(_name, _type) _name,
#define ADD_ENUM_ENTRIES(...) GLUE(ADD_ITEM_HELPER(NARGS(__VA_ARGS__)),(ENUM, __VA_ARGS__))

#define ADD_SINGLE_CONSTRUCTOR(_name, _type) m_ ## _name(nullptr),
#define ADD_MEMBER_CONSTRUCTORS(...) GLUE(ADD_ITEM_HELPER(NARGS(__VA_ARGS__)),(CONSTRUCTOR, __VA_ARGS__))


#define ADD_SINGLE_DESTRUCTOR(_name, _type) delete[] m_ ## _name;
#define ADD_MEMBER_DESTRUCTORS(...) GLUE(ADD_ITEM_HELPER(NARGS(__VA_ARGS__)),(DESTRUCTOR, __VA_ARGS__))

#define ADD_SINGLE_KILL(_name, _type) \
if (m_ ## _name)\
{\
  m_ ## _name[a_index] = m_ ## _name[m_countAlive];\
}
#define ADD_KILL_CODE(...) GLUE(ADD_ITEM_HELPER(NARGS(__VA_ARGS__)),(KILL, __VA_ARGS__))
