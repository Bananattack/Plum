#ifndef PLAIDGADGET_REF_H
#define PLAIDGADGET_REF_H


#ifndef NO_PLAIDGADGET
	#include "memory.h"
#endif


#include "traits.h"


namespace plaidgadget
{
	//RefCount should be an efficient integer type.  No need to regulate size.
	typedef unsigned int RefCount;


	/*
		This class is more efficient, though by no means necessary, to use
			in combination with the Ref<T> class below.
			Multiple-inheritance can be used if another base is desired.

		Unlike other types, multiple Ref<T> can be instantiated from a pointer
			to a RefCounted without issues of safety.
			Derivatives of RefCounted can even dispense Refs to themselves!

		Lastly, a Ref with no template parameter is a Ref< RefCounted > and
			useful when a forward-declared class needs to be shared.
			Simply extend RefCounted from that class and optionally keep a raw
			pointer or reference in the holding object for convenience.


		Errata:  It is safe to cast a Ref to a RefCounted to one of some other
			parent type (say, Ref<plaidgadget::Reflected>) that does not extend
			RefCounted.  The private _surrogate function is used to ensure this.
	*/
	class RefCounted
	{
	public:
		RefCounted() : _refs(0) {}

		//Use these only with great care; usually Ref<T> is safer and easier.
		void _retain()             {++_refs;}
		void _release()            {if (_refs) if (!--_refs) delete this;}
		RefCount _refCount() const {return _refs;}

	protected:
		// It's best practice to make destructors protected in these types.
		// But, if you don't, I doubt anyone will make a fuss about it.
		virtual ~RefCounted() {}

	private:
		template<typename T> friend class Ref;
		friend class RefHelper;
		virtual bool _surrogate() {return false;}
		RefCount _refs;
	};


	/*
		A "surrogate" RefCounted that does the counting for objects that don't
			extend the RefCounted class.  DO NOT EXTEND.
	*/
	class RefHelper : public RefCounted
		{public: virtual ~RefHelper() {}
		private: template<typename T, int I> friend struct RefCountPolicy;
		RefHelper() {} virtual bool _surrogate() {return true;}};


	/*
		A bit of template magic.  Proceed for the Ref class itself.
	*/
	template<typename T, int> struct RefCountPolicy
		{static RefCounted *get(T *t) {return new RefHelper;}};
	template<typename T> struct RefCountPolicy<T, 1>
		{static RefCounted *get(RefCounted *t) {return t;}};


	/*
		Plaidgadget's Ref<T> class is the basis of a lightweight reference
			counting system used for memory management in the engine and any
			other applications which wish to utilize it.

		A common trick is to code classes with private inner classes holding any
			member variables, and a smart pointer to the inner class as the sole
			member of the enclosing class.  The outer class becomes a reference
			to the inner one, and can be copied and destroyed at low cost.

		Notably it's not portably possible to use a Ref as an "opaque pointer",
			as the Ref template is instantiated with instructions to delete an
			incomplete type when the last one is destroyed.  This might cause
			link errors or undefined behavior if allowed to compile.
			(You should get a warning, though...)
	*/
	template<typename T = RefCounted>
	class Ref
	{
	private:
		//The policy, which varies between RefCounted and regular values of T.
		typedef RefCountPolicy<T, Relation<T, RefCounted>::Derives> Policy;

	public:
		//Constructors: explicit so you know when you use them!
		explicit Ref()       : item(NULL),  ref(NULL) {}
		explicit Ref(T *ptr) : item(ptr)
			{if (ptr) {ref=Policy::get(ptr); retain();} else ref=NULL;}

		//Static cast from subclass types
		template<typename D>
		Ref(const Ref<D> &o) : item(o.item), ref(o.ref) {retain();}

		//A more explicit way of constructing a null reference
		static Ref Null()   {return Ref();}

		//Explicit null check
		bool null() const   {return !item;}

		//Copying just retains
		Ref(const Ref &other)  : item(other.item), ref(other.ref) {retain();}

		//Destruction releases this Ref
		~Ref() {release();}

		//Assignment and copying - retain before release !!!
		Ref &operator=(const Ref &o)
			{o.retain(); release(); item=o.item; ref=o.ref; return *this;}
		Ref &operator=(T *ptr) {Ref tmp(ptr); (*this) = tmp; return *this;}

		//Use this in lieu of dynamic_cast to keep Refs coherent
		//    COULD RETURN NULL--BE CAREFUL!
		template<typename D>
		Ref<D> dynamicCast() const
		{
			D *casted = dynamic_cast<D*>(item);
			if (casted || !item) return Ref<D>(casted, ref);
			return Ref<D>(); // (if dynamic_cast failed)
		}

		//And a static cast, for friendly compile errors when misused.
		template<typename D>
		Ref<D> staticCast() const
		{
			//Both types should either be RefCounted or not RefCounted...
			return Ref<D>(static_cast<D*>(item), ref);
		}

		//Comparisons
		bool operator< (const Ref &other) const  {return (item <  other.item);}
		bool operator> (const Ref &other) const  {return (item >  other.item);}
		bool operator<=(const Ref &other) const  {return (item <= other.item);}
		bool operator>=(const Ref &other) const  {return (item >= other.item);}
		bool operator==(const Ref &other) const  {return (item == other.item);}
		bool operator!=(const Ref &other) const  {return (item != other.item);}
		bool operator< (const T*   other) const  {return (item <  other);}
		bool operator> (const T*   other) const  {return (item >  other);}
		bool operator<=(const T*   other) const  {return (item <= other);}
		bool operator>=(const T*   other) const  {return (item >= other);}
		bool operator==(const T*   other) const  {return (item == other);}
		bool operator!=(const T*   other) const  {return (item != other);}

		//Query remaining references
		RefCount count() const   {if (!ref) return 0; return ref->_refs;}
		bool last() const        {return ref && (ref->_refs==1);}

		//Emulate behavior of pointers
		operator T*  ()             const  {return item;}
		T *operator->()             const  {return item;}
		T &operator* ()             const  {return *item;}
		T &operator[](size_t index) const  {return item[index];}

	private:
		template<typename D> friend class Ref;
		Ref(T *_item, RefCounted *_ref)  {item = _item; ref = _ref; retain();}

		void retain() const
		{
			if (ref) ++ref->_refs;
		}
		void release()
		{
			if (!ref || --ref->_refs) return;
			if (ref->_surrogate()) delete item;
			delete ref; item=NULL; ref=NULL;
		}

	private:
		//Members.
		T *item;
		RefCounted *ref;
	};
}


#endif // PLAIDGADGET_REF_H
