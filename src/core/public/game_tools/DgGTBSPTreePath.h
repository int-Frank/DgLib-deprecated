#ifndef DGGTBSPTREEPATH_H
#define DGGTBSPTREEPATH_H

#include <vector>

namespace Dg
{
  namespace GT
  {
    class BSPTreePath
    {
    public:

      BSPTreePath()
        : m_pBelow(nullptr)
        , m_pAbove(nullptr)
      {}

      ~BSPTreePath()
      {
        delete m_pBelow;
        delete m_pAbove;
      }

      BSPTreePath(BSPTreePath const & a_other)
        : m_pBelow(nullptr)
        , m_pAbove(nullptr)
      {
        Init(a_other);
      }

      BSPTreePath & operator=(BSPTreePath const & a_other)
      {
        if (this != &a_other)
        {
          Init(a_other);
        }
        return *this;
      }

      bool IsLeaf() const
      {
        return m_pBelow == nullptr && m_pAbove == nullptr;
      }

      BSPTreePath const * Below() const { return m_pBelow; }
      BSPTreePath const * Above() const { return m_pAbove; }

      std::vector<std::vector<bool>> GetSinglePaths() const
      {
        std::vector<std::vector<bool>> result;
        result.push_back(std::vector<bool>());
        OutputPath(this, 0, result);
        return result;
      }

    private:

      static void OutputPath(BSPTreePath const * a_path, size_t a_ind, std::vector<std::vector<bool>> & a_out)
      {
        BSPTreePath const * pBelow(a_path->Below());
        BSPTreePath const * pAbove(a_path->Above());

        if (pBelow && pAbove)
        {
          std::vector<bool> newPath(a_out[a_ind]);
          a_out.push_back(newPath);
          size_t aboveInd = a_out.size() - 1;

          a_out[a_ind].push_back(false);
          OutputPath(pBelow, a_ind, a_out);

          a_out[aboveInd].push_back(true);
          OutputPath(pAbove, aboveInd, a_out);
        }
        else if (pBelow)
        {
          a_out[a_ind].push_back(false);
          OutputPath(pBelow, a_ind, a_out);
        }
        else if (pAbove)
        {
          a_out[a_ind].push_back(true);
          OutputPath(pAbove, a_ind, a_out);
        }
      }

      void Init(BSPTreePath const & a_other)
      {
        delete m_pBelow;
        m_pBelow = nullptr;

        delete m_pAbove;
        m_pAbove = nullptr;

        if (a_other.m_pBelow)
        {
          m_pBelow = new BSPTreePath(*(a_other.m_pBelow));
        }
        if (a_other.m_pAbove)
        {
          m_pAbove = new BSPTreePath(*(a_other.m_pAbove));
        }
      }

    private:

      BSPTreePath *           m_pBelow;
      BSPTreePath *           m_pAbove;
    };
  }
}

#endif