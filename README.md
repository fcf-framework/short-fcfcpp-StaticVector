# short-fcfcpp-StaticVector
A CPP class that stores data in internal data fields and, if necessary, allocates memory from the heap when there is insufficient memory

---
*Available links for this library:*
* Article on dev.to: https://dev.to/vladimirm/c-vector-with-the-possibility-of-storage-in-data-in-the-memory-of-the-object-41k4
---


If the memory is released in a heap, then the size of the buffer is always distinguished by fixed blocks. The size of the block is determined by the following formula

<img src="https://github.com/fcf-framework/short-fcfcpp-StaticVector/blob/main/README-sources/heap-func.png?raw=true"/>

For superly large values, the size of the buffer can be calculated according to the second function, which has a fixed increase in the size of the buffer when calculating the next step and is used if the resulting sizes of the buffer exceeds the value of MaxPow

MaxPowStepSize step size is determined as the difference in the steppe function of the following after the MaxPow and its previous value.

F(Laststep) - F(Laststep -1)

And MaxPowStart is the first value of the degree function that follows the MaxPow parameter.


### Example

Class application similarly `std::vector<typename Ty>`

```c++
fcf::StaticVector<int, 32> v; 
for(int j = 0; j < a_vecSize; ++j) {
  v.push_back(j); 
} 

for(auto it = v.begin(); it = v.end(); ++it) { 
  std::cout << *it << std::endl;
} 
```


### Description of types and methods

#### Class fcf::StaticVector

##### Description template
`template<typename Ty, unsigned int StaticSize=16, unsigned int OffsetSize=2, unsigned int StepSize=2, unsigned int StepModuleInt=2, unsigned int StepModuleFrac=0, unsigned int MaxPow = 0 >`<br>
`class StaticVector;`


* `typename Ty` - Type of stored elements
* `unsigned int StaticSize` - Maximum number of elements stored in the internal buffer
* `unsigned int OffsetSize` - The offset size used in the calculation function
* `unsigned int StepSize` - The step size used in the calculation function
* `unsigned int StepModuleInt` - The integer part of the StepModule parameter used in the calculation function
* `unsigned int StepModuleFrac` - Fractional part of the StepModule parameter number used in the calculation function
* `unsigned int MaxPow` - The limit value after which the buffer size calculation goes into linear form and a simple increment is performed. If the value is 0, but this functionality is not used


##### Types

* `typedef Ty value_type` - item type
* `typedef size_t size_type` - key type
* `typedef IndexIterator<StaticVector&, Ty&>             iterator;` - iterator

* `typedef IndexIterator<const StaticVector&, const Ty&> const_iterator` - const iterator

##### Methods & constructors

* `StaticVector()` - Default constructor
*  `StaticVector(size_t a_size)` - Designer with the initial initialization of subsidiary elements.
	* size_t a_size - The number of initialized elements
* `StaticVector(const StaticVector& a_source)` - Copy constructor.
* `StaticVector(std::initializer_list<Ty> a_list)` - Constructor receiver initialization list. The initialization list should contain a list of subsidiaries.
* `StaticVector(size_t a_size, const Ty& a_fill)` -Designer filling with the same elements. 
	* `size_t a_size` - The number of initialized elements
	* `const Ty& a_fill` - Initial value
* `StaticVector& operator=(const StaticVector& a_source)` - Assignment operator
* `Ty& operator[](size_t a_index)` - Operator access to a subsidiary on index
* `Ty& operator[](size_t a_index) const` - Operator access to a subsidiary on index
* `iterator begin()` - Returns the begin iterator
* `iterator end()` - Returns the end iterator
* `const_iterator cbegin() const` - Returns the begin const iterator
* `const_iterator cend() const` - Returns the end const iterator
* `size_t size() const` - Returns the number of subsidiaries.
* `size_t capacity() const` - Returns the size of the inner buffer
* `bool empty() const` - Returns true if the container is empty
* `void clear(bool a_notReduce = false)`- Cleans the container
	* `bool a_notReduce = false` - If the argument is equal to true, then a decrease in the buffer is not performed.
* `void resize(size_t a_size, bool a_notReduce = false)` - Changes the size of the container
	* `bool a_notReduce = false` - If the argument is equal to true, then a decrease in the buffer is not performed.
* `void push_back(const Ty& a_value)` - Adds the element to the end of the container.
* `void pop_back(bool a_notReduce = false)` - Removes the last element from the container
	* `bool a_notReduce = false` - If the argument is equal to true, then a decrease in the buffer is not performed.
