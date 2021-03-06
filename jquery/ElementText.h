#ifndef EMP_JQ_ELEMENT_TEXT_H
#define EMP_JQ_ELEMENT_TEXT_H

///////////////////////////////////////////////////////////////////////////////////////////
//
//  Manage a section of the current web page
//

#include <functional>
#include <string>
#include <vector>

#include "emscripten.h"

#include "Element.h"

namespace emp {
namespace JQ {

  class ElementText : public Element {
  private:
    std::vector<std::string> string_set;
    std::vector<std::function<void()>> fun_set;

  public:
    ElementText(const std::string & in_name, Element * in_parent)
      : Element(in_name, in_parent) { ; }
    ~ElementText() { ; }

    // Do not allow Managers to be copied
    ElementText(const ElementText &) = delete;
    ElementText & operator=(const ElementText &) = delete;

    virtual bool IsText() const { return true; }

    void ClearText() { string_set.resize(0); }

    Element & Append(const std::string & in_text) {
      string_set.push_back(in_text);
      SetModified();
      return *this;
    }

    Element & Append(const std::function<std::string()> & in_fun) {
      int string_id = string_set.size();
      string_set.push_back(in_fun());
      std::string & cur_string = string_set[string_id];
      fun_set.push_back( [&cur_string, in_fun](){ cur_string = in_fun(); }  );
      return *this;
    }

    void UpdateNow() {
      // Collect all function outputs
      for (auto & cur_fun : fun_set) cur_fun();

      // Concatenate the text.
      std::string text;
      for (auto & cur_str : string_set) { text += cur_str; }
      EM_ASM_ARGS({
          var elem_name = Pointer_stringify($0);
          var text = Pointer_stringify($1);
          $( '#' + elem_name ).html(text);
        }, GetName().c_str(), text.c_str() );
    }

    void PrintHTML(std::ostream & os) {
      std::string text;
      for (auto & cur_str : string_set) { text += cur_str; }
      os << text << std::endl;
    }
  };

};
};

#endif
