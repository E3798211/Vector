#ifndef ARRAY_HPP_INCLUDED
#define ARRAY_HPP_INCLUDED

#include "Debug/DebugLib.h"

template<typename data_T>
class Array
{
private:

    /// Array itself
    data_T* array_      = nullptr;

    /// Array's capacity
    size_t capacity_    = 0;

    /// Amount of elements in array
    size_t n_elements_  = 0;

    /// Swaps elements of two Arrays<data_t>
    int Swap(Array<data_T>& that);

    /// Resizes array_
    bool Resize(size_t new_size);

public:

    /// Tmp value
    data_T  garbage_collector_  = 0;

    /// Default constructor
    explicit Array();

    /// Constructors with params
    explicit Array(size_t parameters_expected); // <-- Array a = 10;                forbidden

/*
    /// Copying constructor
    template<typename data_G>
    Array(const Array<data_G>& that) = delete;  // <-- Array a = b; Array a(b);     forbidden
*/

    /// Copying constructor
    Array(const Array<data_T>& that);  // <-- Array a = b; Array a(b);              accepted

    /// Move constructor
    Array(Array<data_T>&& that):
        array_      (that.GetArray()),
        capacity_   (that.GetCapacity()),
        n_elements_ (that.GetNElements())
    {
        PrintInfo("Move constructor\n");
        if(&that != this)
        {
            that.SetArray(nullptr);
        }
    }

    /// Destructor
    ~Array();

// =============================
// Operators.

    /// Compare operator
    template<typename data_G>
    bool operator==(const Array<data_G>& that);

    /// []
    data_T& operator[](double index);

    /// Operator new (for move)
    void* operator new(size_t size, Array<data_T>* place)
    {
        return place;
    }

    /// Move assignment
    Array<data_T>& operator=(Array<data_T>&& that)
    {
        PrintInfo("Move assignment\n");

        Swap(that);
    }

    /// Copy assignment
    Array<data_T>& operator=(const Array<data_T>& that)
    {
        PrintInfo("Copy assignment\n");

        Array<data_T> tmp_object(that);         // <-- Copy constructor, deep copying
        Swap(tmp_object);

        return *this;
    }

    /// "push" operator
    bool operator<<(data_T new_elem);

    /// "pop"  operator
    bool operator>>(data_T& new_elem);


// =============================
// Members.

    /// Gets capacity_
    size_t GetCapacity() const
    {
        return capacity_;
    }

    /// Gets n_elements_
    size_t GetNElements() const
    {
        return n_elements_;
    }

    /// Gets array_
    data_T* GetArray() const
    {
        return array_;
    }

    /// Sets capacity_
    size_t SetCapacity(size_t new_capacity)
    {
        capacity_ = new_capacity;
        return capacity_;
    }

    /// Sets n_elements_
    size_t SetNElements(size_t new_n_elements)
    {
        n_elements_ = new_n_elements;
        return n_elements_;
    }

    /// Sets array_
    data_T* SetArray(data_T* new_array)
    {
        array_ = new_array;
        return array_;
    }

    /// Prints all info about object
    void PrintAll();
};

// =====================================================================================
// Constructors

template<typename data_T>                                                               // !
Array<data_T>::Array():
    array_      (nullptr),
    capacity_   (0),
    n_elements_ (0)
{
    PrintInfo("Default constructor\n");
}

template<typename data_T>
Array<data_T>::Array(size_t parameters_expected):
    capacity_   (parameters_expected),
    n_elements_ (parameters_expected)
{
    PrintInfo("Constructor with parameters\n");
    try
    {
        array_ = new data_T [parameters_expected];
    }
    catch(std::bad_alloc& ex)
    {
        PrintErr("Failed to allocate memory in constructor with parameters\n");
        return;
    }
}

template<typename data_T>
Array<data_T>::Array(const Array<data_T>& that):
    capacity_   (that.GetCapacity()),
    n_elements_ (that.GetNElements())
{
    PrintInfo("Copy constructor\n");

    try
    {
        array_ = new data_T [capacity_];
    }
    catch(std::bad_alloc& ex)
    {
        PrintErr("Failed to allocate memory in copy constructor\n");
        return;
    }

    for(int i = 0; i < n_elements_; i++)
        array_[i] = that.GetArray()[i];
}

template<typename data_T>
Array<data_T>::~Array()                                                                 // !
{
    PrintInfo("Destructor\n");

    PrintAll();
    delete [] array_;

    PrintInfo("Deleted ==\n");
}

// =====================================================================================
// Operators

template<typename data_T>
template<typename data_G>
bool Array<data_T>::operator==(const Array<data_G>& that)                               // !
{
    return false;
}

template<typename data_T>
data_T& Array<data_T>::operator[](double index)                                         // !
{
    if(index < 0)
    {
        PrintErr("Index is below zero\n");
        return garbage_collector_;
    }
    if((size_t)index >= capacity_ - 1)
    {
        PrintErr("No such elements\n");
        return garbage_collector_;
    }
    return array_[(size_t)index];
}

template<>
double& Array<double>::operator[](double index)
{
    if((size_t)index < 1)
    {
        PrintErr("Index is below 2\n");
        return garbage_collector_;
    }
    if((size_t)index >= capacity_ - 1)          // index = 9.1, capacity_ = 10     <-- forbidden
    {
        PrintErr("Not enough knot points\n");
        return garbage_collector_;
    }

//  Counting by Catmull-Rom
//
//
//                              |   0       1       0       0   |       | p(i-2) |
//                              |                               |       |        |
//                              |   -U      0       U       0   |       | p(i-1) |
//  p(t) = [1, t, t^2, t^3] *   |                               |   *   |        |
//                              |   2U     U-3    3-2U      -U  |       | p(i)   |
//                              |                               |       |        |
//                              |   -U     2-U     U-2      U   |       | p(i+1) |
//
//
// Here index = t
//
// U = 0.5. For now :)
//

    double U = 0.5;
    double t = index - (size_t)index;

    size_t x1 = (size_t)index - 1;  // p(i-2)
    double y1 = array_[x1];
    size_t x2 = (size_t)index;      // p(i-1)
    double y2 = array_[x2];
    size_t x3 = (size_t)index + 1;  // p(i)
    double y3 = array_[x3];
    size_t x4 = (size_t)index + 2;  // p(i+1)
    double y4 = array_[x4];


    double p1 =                  y2                      ;
    double p2 =  -U*y1 +                      U*y3       ;
    double p3 = 2*U*y1 + (U - 3)*y2 + (3 - 2*U)*y3 - U*y4;
    double p4 =  -U*y1 + (2 - U)*y2 +   (U - 2)*y3 + U*y4;

    garbage_collector_ = p1 + p2*t + p3*t*t + p4*t*t*t;             // <-- couldn't return temporary variable
    return garbage_collector_;
}


template<typename data_T>
bool Array<data_T>::operator<<(data_T new_elem)
{
    if(capacity_ <= 0)
        try
        {
            Resize(2);
        }
        catch(std::bad_alloc& ex)
        {
            PrintErr("Failed to allocate memory in operator<<\n");
            return false;
        }
    if(n_elements_ >= capacity_)
        try
        {
            Resize(capacity_*2);
        }
        catch(std::bad_alloc& ex)
        {
            PrintErr("Failed to allocate memory in operator<<\n");
            return false;
        }

    array_[n_elements_++] = new_elem;

    return true;
}

template<typename data_T>
bool Array<data_T>::operator>>(data_T& new_elem)
{
    if(n_elements_ <= 0)
    {
        PrintErr("Array is completely empty\n");
        return false;
    }

    new_elem = array_[--n_elements_];

    if(n_elements_ < capacity_/2)
        try
        {
            Resize(capacity_/2);
        }
        catch(std::bad_alloc& ex)
        {
            PrintErr("Failed to allocate memory in operator>>\n");
            return false;
        }

    return true;
}

// =====================================================================================
// Functions

template<typename data_T>
bool Array<data_T>::Resize(size_t new_size)
{
    size_t min_size = (capacity_ <  new_size) ? capacity_ : new_size;

    data_T* new_arr = nullptr;
    try
    {
        new_arr = new data_T [new_size];
    }
    catch(std::bad_alloc& ex)
    {
        throw;
    }

    for(size_t i = 0; i < min_size; i++)
        new_arr[i] = array_[i];

    delete array_;
    array_ = new_arr;
    capacity_ = new_size;

    return true;
}

template<typename data_T>
void Array<data_T>::PrintAll()
{
    std::cout << "this     = " << this        << "\n";
    std::cout << "capacity = " << capacity_   << "\n";
    std::cout << "n_elemen = " << n_elements_ << "\n";
    std::cout << "array_   = " << array_      << "\n";

    for(size_t i = 0; i < n_elements_; i++)
        std::cout << "arr[" << i << "]\t= " << array_[i] << "\n";
}

template<typename data_T>
int Array<data_T>::Swap(Array<data_T>& that)
{
    data_T* tmp_arr = nullptr;

    tmp_arr = this->array_;
    this->array_ = that.GetArray();
    that.SetArray(tmp_arr);

    size_t tmp = 0;

    tmp = this->capacity_;
    this->capacity_ = that.GetCapacity();
    that.SetCapacity(tmp);

    tmp = this->n_elements_;
    this->n_elements_ = that.GetNElements();
    that.SetNElements(tmp);

    return tmp;
}


#endif // ARRAY_HPP_INCLUDED
