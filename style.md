Style Guide
-----------

Here are some dumb rules I think about sometimes. It'd be nice to keep to them.

C++

* Indent with 4 spaces, no tabs.
* Prefer `camelCase` for naming variables and functions.
* Prefer `PascalCase` for naming types and constants.
* Prefer `CAPS_WITH_UNDERSCORES` for macros.
* Prefer `//` to `/* ... */` for comments, except when commenting out a large chunk of code.
* Braces should be on the next line for control structures.
* No spaces between keywords and an open parentheses `(`.
* Add a spaces or newlines after `,` and `;`
* Add spaces between operators and their operands, except `.`, `->`, `::`, `[]`, `()` and unary operators.
* Avoid unnecessary parentheses, learn operator precedences.
* When you can, try to order includes and symbols lexicographically -- by ascending size and by alphabetical order.
* Avoid macros except when absolutely necessary!
* Prefer `enum` to `const int` declarations when there are several, related integer constants.
* Actually use handy C++ features, like templates and references!
* Use C++11 features, like `auto`, lambdas (stateless ones work with C libs), range-based `for` loops, pointer types (`shared_ptr`, `weak_ptr` and `unique_ptr`).
* Use `const` qualifiers (const methods, pointers, references) often! If you're not mutating particular data, make that fact explicit!
* Use `unsigned` types whenever negative numbers are not necessary.
* User `enum` types whenever there are a restricted set of integer values that map to specific meanings.
* Never use `using` to import other namespaces.
* Keep as much code as possible in `namespace plum`
* Avoid adding any global variables or class-static variables, and keep them to the smallest scope they are needed.
* Avoid pointers when possible, and avoid writing code that uses `new`/`delete`. If you absolutely need to, use `<memory>` pointer types, or be extremely careful.
* Avoid nullable types
* Avoid inheritance
* Avoid virtual dispatch
* Prefer references to pointers -- they aren't nullable, or rebindable, and thus they are much safer.
* Create things as value types when possible, avoid unnecessary copying by using references.
* Separate interface from implementation, by using a variant of pimpl-idiom in types: `class Impl; std::shared_ptr<Impl> impl;`
* Avoid pure abstract interfaces (which require virtual dispatch and pointers, are nullable, and can blow up at runtime if a pure virtual call slips through), and prefer the pimpl idiom.
* Prefer `T getFoo();` / `setFoo(T value);` for value attributes. Prefer `T& foo()` for accessors to mutable references.
* If a `class` contains only members with trivial public get/set of private members, maybe make it `struct` with all public members and no get/set.
* Don't `_prefix` names, `suffix_` them instead -- unlike the former, the latter isn't reserved!
* Use anonymous namespaces whenever a global symbol is only needed for one file. Don't use C-style file-scoping `static`.
* Try to keep things out of the headers, unless they're needed there. Compile times don't need to be slowed down further.

I dunno, there's probably more to it, but hopefully that's a good guide.