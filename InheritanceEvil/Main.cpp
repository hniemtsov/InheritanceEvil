#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cassert>
using namespace std;

/* 1)
using object_t = int;
void draw(const object_t& x, ostream& out, size_t position)
{ out << string(position, ' ') << x << endl; }
*/

//// Replace with template
////void draw(const string& x, ostream& out, size_t position)
////{	out << string(position, ' ') << x << endl;}
////
////void draw(const int& x, ostream& out, size_t position)
////{ out << string(position, ' ') << x << endl; }
////
template<typename T>
void draw(const T& x, ostream& out, size_t position)
{	out << string(position, ' ') << x << endl; }

/* 2) Just wrap int into class
class object_t {
public:
	object_t(const int& x) : self_(x)
	{}
	friend void draw(const object_t& x, ostream& out, size_t position)
	{ draw (x.self_, out, position); }
private:
	int self_;
};
*/
// Add indirection inside and heap alocate int
class object_t {
public:
////	object_t(string x) : self_(new string_model_t(move(x)))
////	{		cout << "ctor str x=" << ((string_model_t*)self_.get())->data_ << endl;	}
////	object_t(int x): self_(new int_model_t(move(x))) 
////	{		cout << "ctor x=" << x << endl; 	}
	template< typename T>
	object_t(T x) : self_(make_shared<model<T>>(move(x)))
	{
		cout << "ctor x=" << endl;
	}
/* 
	object_t(const object_t& x) : self_(x.self_->copy()) 
	{		cout << "copy" << endl; 	}
	object_t(object_t&&) noexcept = default;
	object_t& operator=(const object_t& x)
	{ 
		cout << "op=" << endl; 
		object_t tmp(x); *this= move(tmp); return *this; 
	}
	object_t& operator=(object_t&&) noexcept = default;
*/
	friend void draw(const object_t& x, ostream& out, size_t position)
	{ x.self_->draw_(out, position); }
private:
	struct concept_t {
		virtual ~concept_t() = default;
/*		virtual concept_t* copy() const = 0; */
		virtual void draw_(ostream&, size_t) const = 0;
	};
////	struct int_model_t : concept_t{
////		int_model_t(int x) : data_(move(x)){}
////		concept_t* copy() const { return new int_model_t(*this); }
////		void draw_(ostream& out, size_t position) const
////		{ draw(data_, out, position); }
////		int data_;
////	};
////	struct string_model_t : concept_t {
////		string_model_t(string x) : data_(move(x)) {}
////		concept_t* copy() const { return new string_model_t(*this); }
////		void draw_(ostream& out, size_t position) const
////		{			draw(data_, out, position);		}
////		string data_;
////	};
	template<typename T>
	struct model : concept_t {
	model(T x) : data_(move(x)) {}
/*	concept_t* copy() const { return new model(*this); }*/
	void draw_(ostream& out, size_t position) const
	{			draw(data_, out, position);		}
	T data_;
};

	shared_ptr<const concept_t> self_;
};


using document_t = vector<object_t>;
void draw(const document_t& x, ostream& out, size_t position)
{
	out << string(position, ' ') << "<document>" << endl;
	for (const auto& e : x) draw(e, out, position + 2);
	out << string(position, ' ') << "</document>" << endl;
}

using history_t = vector<document_t>;
void commit(history_t& x) {
	assert(x.size()); 
	x.push_back(x.back()); 
}
void undo(history_t& x) { 
	assert(x.size());
x.pop_back();
}
document_t& current(history_t& x) { 
	assert(x.size()); 
	return x.back(); 
}

object_t func()
{
	object_t result = 5;
	return result;
}

class my_class_t {

};

void draw(const my_class_t&, ostream& out, size_t position)
{
	out << string(position, ' ') << "my_class_t" << endl;
}

int main()
{

	document_t doc;
	doc.push_back(2);
	auto s1 = doc[0];
	s1 = "455";


//	object_t x = 0;
//	x = func();

//	object_t tt1(0);
//	object_t tt2 = tt1;
///////////////////////////
//	document_t document;
//	document.reserve(4);
//	
//	document.emplace_back(0);
//	document.emplace_back(string("Hello!"));
//	document.emplace_back(document);
//	document.emplace_back(2);
//	document.emplace_back(my_class_t());
//
//	reverse(document.begin(), document.end());
//
//	draw(document, cout, 0);
////////////////////////////////////
	shared_ptr<const int> pI = make_shared<const int>(3);
	//*pI = 8;
	pI = shared_ptr<int>(new int(6));

	history_t h(1);
	current(h).emplace_back(10);
	current(h).emplace_back(string("Hello!"));

	draw(current(h), cout, 0);
	cout << "---------------------------------------\n";

	commit(h);

	current(h).emplace_back(current(h));
	current(h).emplace_back(my_class_t());
	current(h)[1] = string("World");

	draw(current(h), cout, 0);
	cout << "---------------------------------------\n";

	undo(h);
	draw(current(h), cout, 0);


}