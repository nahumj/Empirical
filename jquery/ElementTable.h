#ifndef EMP_JQ_ELEMENT_TABLE_H
#define EMP_JQ_ELEMENT_TABLE_H

//////////////////////////////////////////////////////////////////////////////////////////
//
//  Manage a single table.
//

#include <functional>
#include <string>
#include <vector>

#include "emscripten.h"

#include "Element.h"

namespace emp {
namespace JQ {

  class ElementTable : public Element {
  private:
    emp::JQ::Table base_info;

  public:
    ElementTable(const emp::JQ::Table & in_info, Element * in_parent)
      : Element(in_info.name, in_parent), base_info(in_info) { ; }
    ~ElementTable() { ; }

    // Do not allow Managers to be copied
    ElementTable(const ElementTable &) = delete;
    ElementTable & operator=(const ElementTable &) = delete;

    void UpdateNow() { ; }
    void PrintHTML(std::ostream & os) { ; }

  };

};
};

#endif
