#ifndef EMP_JQ_ELEMENT_H
#define EMP_JQ_ELEMENT_H

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Base class for a single element on a web page (a paragraph, a table, etc.)
//

#include <emscripten.h>
#include <string>

#include "../tools/assert.h"
#include "../tools/string_utils.h"

#include "events.h"
#include "UI_base.h"

namespace emp {
namespace JQ {

  class Element {
  protected:
    bool modified;      // Has this element been modified since the last update? 
    std::string name;

    // Track hiearchy
    Element * parent;
    std::vector<Element *> children;

    // If an Append doesn't work with the currnet class, forward it to the parent!
    template <typename FORWARD_TYPE>
    Element & ForwardAppend(FORWARD_TYPE && arg) {
      emp_assert(parent != nullptr);
      return parent->Append(std::forward<FORWARD_TYPE>(arg));
    }

  public:
    Element(const std::string & in_name, Element * in_parent)
      : modified(true), name(in_name), parent(in_parent)
    {
      emp_assert(name.size() > 0);  // Make sure a name was included.
      // @CAO ensure the name consists of just alphanumeric chars (plus '_' & '-'?)
      Register(this);
    }
    Element(const Element &) = delete;
    virtual ~Element() {
      // Recursively delete children.
      for (Element * cur_element : children) { delete cur_element; }
    }
    Element & operator=(const Element &) = delete;

    // Functions to access current state
    bool IsModified() const { return modified; }

    virtual bool IsText() const { return false; }

    const std::string & GetName() const { return name; }
    Element * GetParent() { return parent; }

    // Functions to access children
    int GetNumChildren() const { return children.size(); }
    Element & GetChild(int id) {
      emp_assert(id >= 0 && id < children.size());
      return *(children[id]);
    }
    const Element & GetChild(int id) const {
      emp_assert(id >= 0 && id < children.size());
      return *(children[id]);
    }

    // Function to make current element as modified and pass information up to parents.
    void SetModified() {
      if (modified) return;   // Stop recursion if already not up-to-date.
      modified = true;
      if (parent) parent->SetModified();
    }

    // Register is used to lookup classes by name.  Should exist in classes that manage
    // multiple element; below is the default version.
    virtual bool Register(Element * new_element) {
      if (!parent) return false;  // Top level should always have an override for Register()

      parent->Register(new_element);
      return true;
    }


    // UpdateNow() refreshes the document immediately (and should only be called if that's okay!)
    virtual void UpdateNow() = 0;

    // Update() refreshes the document as soon as it's ready.
    void Update() {
      std::function<void(void)> update_fun = [this](){ this->UpdateNow(); };
      OnDocumentReady( update_fun );

      // OnDocumentReady( [this](){ this->UpdateNow(); } );
      // OnDocumentReady( std::function<void()> ( std::bind(&Element::UpdateNow, this) ) );
    }



    // By default, elements should forward unknown inputs to their parents.

    virtual Element & Append(const std::string & in_text) { return ForwardAppend(in_text); }
    virtual Element & Append(const std::function<std::string()> & in_fun) {
      return ForwardAppend(in_fun);
    }
    virtual Element & Append(emp::JQ::Button info) { return ForwardAppend(info); }
    virtual Element & Append(emp::JQ::Image info) { return ForwardAppend(info); }
    virtual Element & Append(emp::JQ::Table info) { return ForwardAppend(info); }


    // Convert arbitrary inputs to a string and try again!
    virtual Element & Append(char in_char) { return Append(emp::to_string(in_char)); }
    virtual Element & Append(double in_num) { return Append(emp::to_string(in_num)); }
    virtual Element & Append(int in_num) { return Append(emp::to_string(in_num)); }


    // Setup all speciality operators to also have an append varient.
    template <typename VAR_TYPE>
    Element & AppendVar(VAR_TYPE & var) {
      // return Append( [&var](){ return emp::to_string(var); } );
      return Append( emp::JQ::Var(var) );
    }

    Element & AppendButton(const std::function<void()> & in_cb,
                           const std::string & in_label="",
                           const std::string & in_name="") {
      return Append(emp::JQ::Button(in_cb, in_label, in_name));
    }

    Element & AppendImage(const std::string & in_url, const std::string & in_name="") {
      return Append(emp::JQ::Image(in_url, in_name));
    }

    Element & AppendTable(int in_cols, int in_rows, const std::string & in_name="") {
      return Append(emp::JQ::Table(in_cols, in_rows, in_name));
    }


    // Setup << operator to redirect to Append.
    template <typename IN_TYPE>
    Element & operator<<(IN_TYPE && in_val) { return Append(std::forward<IN_TYPE>(in_val)); }



    virtual void PrintHTML(std::ostream & os) = 0;
    void AlertHTML() {
      std::stringstream ss;
      PrintHTML(ss);
      emp::Alert(ss.str());
    }
  };

};
};

#endif
