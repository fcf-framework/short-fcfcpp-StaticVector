#ifndef ___FCF_BASIS__BITS__ITERATOR__INDEX_ITERATOR_HPP___
#define ___FCF_BASIS__BITS__ITERATOR__INDEX_ITERATOR_HPP___

namespace fcf {

  template <typename TRefContainer, typename TRefItem>
  class IndexIterator {
      IndexIterator();
    public:
      typedef typename std::remove_reference<TRefItem>::type* ptr_type;
      typedef TRefItem ref_type;

      inline IndexIterator(TRefContainer a_container, size_t a_index)
        : _c(a_container)
        , _i(a_index) {
      }

      inline bool operator!=(const IndexIterator& a_source) const {
        return _i != a_source._i;
      }

      inline bool operator==(const IndexIterator& a_source) const {
        return _i == a_source._i;
      }

      inline bool operator<(const IndexIterator& a_source) const {
        return _i < a_source._i;
      }

      inline bool operator<=(const IndexIterator& a_source) const {
        return _i <= a_source._i;
      }

      inline bool operator>(const IndexIterator& a_source) const {
        return _i > a_source._i;
      }

      inline bool operator>=(const IndexIterator& a_source) const {
        return _i >= a_source._i;
      }

      inline IndexIterator& operator++() {
        ++_i;
        return *this;
      }

      inline IndexIterator operator++(int) {
        IndexIterator it(*this);
        ++_i;
        return it;
      }

      inline IndexIterator& operator--() {
        --_i;
        return *this;
      }

      inline IndexIterator operator--(int) {
        IndexIterator it(*this);
        --_i;
        return it;
      }

      inline ref_type operator*() {
        return _c[_i];
      }

      inline ptr_type operator->() {
        return &_c[_i];
      }

      inline IndexIterator operator+(size_t a_diff) {
        IndexIterator it(*this);
        it._i += a_diff;
        return it;
      }

      inline IndexIterator operator-(size_t a_diff) {
        IndexIterator it(*this);
        it._i -= a_diff;
        return it;
      }

    protected:
      TRefContainer _c;
      size_t        _i;
  };

} // fcf namespace

#endif // #ifndef ___FCF_BASIS__BITS__ITERATOR__INDEX_ITERATOR_HPP___

#ifndef ___FCF_BASIS__BITS__STATIC_VECTOR__STATIC_VECTOR_HPP___
#define ___FCF_BASIS__BITS__STATIC_VECTOR__STATIC_VECTOR_HPP___

#include <memory>
#include <cmath>

void staticVectorTest();
void staticVectorPushTest();

namespace fcf {

  ///
  /// @class StaticVector<typename Ty, unsigned int StaticSize=16, unsigned int OffsetSize=2, unsigned int StepSize=2, unsigned int StepModuleInt=2, unsigned int StepModuleFrac=0, unsigned int MaxPow=2*4096/sizeof(Ty)>
  /// @brief A class that stores data in internal data fields and, if necessary, allocates memory from the heap when there is insufficient memory
  /// @detail Data storage algorithm
  ///           - If the buffer size (StaticSize) is sufficient to store the data, then the data is stored in internal fields.
  ///             BufferSizeFunction()=StaticVector::capacity() == StaticSize
  ///           - If the internal buffer size is not sufficient, the data will be stored on the heap.
  ///           The buffer size is determined by the closest value of the following formula \n
  ///             StaticVector::capacity() == OffsetSize + SUM(from{0}, to{n}, StepSize * StepModule^{n})\n
  ///             That is, for n==3:\n
  ///               StaticVector::capacity() == OffsetSize + StepSize + StepSize*StepModule + StepSize*StepModule^2 + StepSize*StepModule^3\n
  ///           - If the MaxPow parameter is not equal to 0, then if the buffer size is greater than the MaxPow value, then the calculation
  ///           of subsequent values ​​of the buffer size is performed by adding the difference between the previous (n-1)
  ///           and the previous (n-2) buffer size.
  ///
  ///         Description of template parameters
  ///           - typename Ty - Type of stored elements
  ///           - unsigned int StaticSize - Maximum number of elements stored in the internal buffer
  ///           - unsigned int OffsetSize - The offset size used in the calculation function
  ///           - unsigned int StepSize - The step size used in the calculation function
  ///           - unsigned int StepModuleInt - The integer part of the StepModule parameter used in the calculation function
  ///           - unsigned int StepModuleFrac - Fractional part of the StepModule parameter number used in the calculation function
  ///           - unsigned int MaxPow - The limit value after which the buffer size calculation goes into linear form and a simple increment is performed.
  ///                                   If the value is 0, but this functionality is not used
  ///
  template<typename Ty, unsigned int StaticSize=16, unsigned int OffsetSize=2, unsigned int StepSize=2, unsigned int StepModuleInt=2, unsigned int StepModuleFrac=0, unsigned int MaxPow=/*16*2*4096/sizeof(Ty)*/ 0 >
  class StaticVector {
    friend void ::staticVectorTest();
    friend void ::staticVectorPushTest();

    public:
      typedef Ty value_type;
      typedef size_t size_type;
      typedef IndexIterator<StaticVector&, Ty&>             iterator;
      typedef IndexIterator<const StaticVector&, const Ty&> const_iterator;

      StaticVector()
        : _pdata((Ty*)&_adata[0])
        , _sdata(0)
        , _cdata(StaticSize) {
      }

      ~StaticVector() {
        _destroyMem(_pdata, _sdata);
      }

      StaticVector(size_t a_size)
        : _pdata((Ty*)&_adata[0])
        , _sdata(0) 
        , _cdata(StaticSize){
        _realloc(_getBufferSize(a_size), a_size);
      }

      StaticVector(const StaticVector& a_source)
        : _pdata((Ty*)&_adata[0])
        , _sdata(0)
        , _cdata(StaticSize) {
        _forceRealloc(_getBufferSize(a_source.size()));
        _copyMem(_pdata, a_source.size(), a_source, _sdata);
      }

      StaticVector(std::initializer_list<Ty> a_list)
        : _pdata((Ty*)&_adata[0])
        , _sdata(0) 
        , _cdata(StaticSize) {
        _forceRealloc(_getBufferSize(a_list.size()));
        for(auto it = a_list.begin(); it != a_list.end(); ++it){
          new (&_pdata[_sdata]) Ty(*it);
          ++_sdata;
        }
      }

      StaticVector(size_t a_size, const Ty& a_fill)
        : _pdata((Ty*)&_adata[0])
        , _sdata(0)
        , _cdata(StaticSize) {
        _forceRealloc(_getBufferSize(a_size));
        for(; _sdata < a_size; ++_sdata){
          new (&_pdata[_sdata]) Ty(a_fill);
        }
      }

      StaticVector& operator=(const StaticVector& a_source){
        _forceRealloc(_getBufferSize(a_source.size()));
        _copyMem(_pdata, a_source.size(), a_source, _sdata);
        return *this;
      }

      iterator begin(){
        return iterator(*this, 0);
      }

      const_iterator cbegin() const{
        return const_iterator(*this, 0);
      }

      iterator end(){
        return iterator(*this, _sdata);
      }

      const_iterator cend() const{
        return const_iterator(*this, _sdata);
      }

      Ty& operator[](size_t a_index){
        return _pdata[a_index];
      }

      const Ty& operator[](size_t a_index) const{
        return _pdata[a_index];
      }

      size_t size() const{
        return _sdata;
      }

      size_t capacity() const{
        return _cdata;
      }

      bool empty() const{
        return !_sdata;
      }

      void clear(bool a_notReduce = false){
        if (a_notReduce) {
          _destroyMem(_pdata, _sdata, _sdata);
        } else {
          _realloc(0, 0);
        }
      }

      void push_back(const Ty& a_value){
        if (_cdata <= _sdata){
          _realloc(_getNextSize(_cdata), _sdata);
        }
        new (&_pdata[_sdata]) Ty(a_value);
        ++_sdata;
      }

      void pop_back(bool a_notReduce = false){
        if (_sdata) {
          if (!a_notReduce) {
            size_t prev = _getPreviousSize(capacity());
            if (prev >= _sdata - 1){
              _realloc(prev, _sdata-1);
            } else {
              --_sdata;
              _pdata[_sdata].~Ty();
            }
          } else {
            --_sdata;
            _pdata[_sdata].~Ty();
          }
        }
      }

      void resize(size_t a_size, bool a_notReduce = false) {
        size_t newBufferSize = _getBufferSize(a_size);
        if (a_notReduce && newBufferSize < _cdata) {
          newBufferSize = _cdata;
        }
        _realloc(newBufferSize, a_size);
      }

    protected:
      struct PrebuildPowSizes {
        PrebuildPowSizes() {
          size_t lastValue = StaticSize + 1;
          size_t m = 0;
          for(size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i){
            m = _getPowSize(lastValue);
            values[i] = m;
            lastValue = m + 1;
          }
          max = m;
        }

        bool isp(size_t a_size){
          return max >= a_size;
        }

        bool is(size_t a_size){
          return max >= a_size;
        }

        size_t get(size_t a_size) {
          for(size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i){
            if (values[i] >= a_size) {
              return values[i];
            }
          }
          return _getPowSize(a_size);
        }

        size_t getp(size_t a_size) {
          size_t last = StaticSize;
          for(size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i){
            if (values[i] >= a_size) {
              return last;
            }
            last = values[i];
          }
          return _getPowSize(a_size, 0);
        }

        size_t max;
        size_t values[10];
      };

      static void _initMem(Ty* a_mem, size_t a_size, size_t& a_sdata) {
        new (&a_mem[0]) Ty[a_size]();
        a_sdata += a_size;
      }

      static void _destroyMem(Ty* a_mem, size_t a_size, size_t& a_sdata) {
        a_sdata -= a_size;
        while (a_size) {
          --a_size;
          a_mem[a_size].~Ty();
        }
      }

      static void _destroyMem(Ty* a_mem, size_t a_size) {
        while (a_size) {
          --a_size;
          a_mem[a_size].~Ty();
        }
      }

      template <typename TContainer>
      static void _copyMem(Ty* a_mem, size_t a_size, TContainer& a_container, size_t& a_sdata) {
        for(size_t i = 0; i < a_size; ++i) {
          new (&a_mem[i]) Ty(a_container[i]);
          ++a_sdata;
        }
      }

      void _forceRealloc(size_t a_size){
        const size_t oldCapacity = capacity();

        _destroyMem(_pdata, _sdata);

        if ((char*)_pdata != &_adata[0] && oldCapacity != a_size){
          _mdata.reset();
        }

        _sdata = 0;

        if (a_size <= StaticSize) {
          _pdata = (Ty*)&_adata[0];
          _cdata = StaticSize;
        } else if (oldCapacity != a_size){
          size_t bufferSize = sizeof(Ty)*a_size;
          _mdata.reset(new char[bufferSize]);
          _pdata = (Ty*)_mdata.get();
          _cdata = a_size;
        }
      }

      void _realloc(size_t a_size, size_t a_initSize) {
        if (a_size <= StaticSize) { // if the a_size fits in the internall buffer
          if ((char*)_pdata == &_adata[0]) { // if the current data is in the internal buffer
            if (_sdata > a_initSize) {
              _destroyMem(&_pdata[a_initSize], _sdata - a_initSize, _sdata);
            } else if (a_initSize > _sdata) {
              _initMem(&_pdata[_sdata], a_initSize - _sdata, _sdata);
            }
          } else {
            size_t copyCounter = 0;
            try {
              _copyMem((Ty*)&_adata[0], a_initSize, _pdata, copyCounter);
            } catch(...){
              _destroyMem((Ty*)&_adata[0], copyCounter);
              throw;
            }
            _destroyMem(_pdata, _sdata);
            _pdata = (Ty*)&_adata[0];
            _mdata.reset();
            _sdata = a_initSize;
            _cdata = StaticSize;
          }
        } else {
          if (capacity() == a_size) {
            if (_sdata > a_initSize) {
              _destroyMem(&_pdata[a_initSize], _sdata - a_initSize, _sdata);
            } else if (_sdata < a_initSize) {
              _initMem(&_pdata[_sdata], a_initSize - _sdata, _sdata);
            }
          } else {
            size_t bufferSize = sizeof(Ty)*a_size;
            std::unique_ptr<char[]> uptr(new char[bufferSize]);
            Ty* newPtr                    = (Ty*)uptr.get();
            Ty* oldPtr                    = _pdata;
            size_t oldSize                = _sdata;

            size_t copyCounter = 0;
            try {
              _copyMem(newPtr, std::min(oldSize, a_initSize), oldPtr, copyCounter);
            } catch(...){
              _destroyMem(newPtr, copyCounter);
              throw;
            }
            if (a_initSize > _sdata) {
              try {
                _initMem(&newPtr[oldSize], a_initSize - oldSize, copyCounter);
              } catch(...){
                _destroyMem(newPtr, copyCounter);
                throw;
              }
            }
            _destroyMem(_pdata, oldSize);
            _mdata.swap(uptr);
            _sdata = copyCounter;
            _pdata = newPtr;
            _cdata = a_size;
          }
        }
      }

      static size_t _getBufferSize(size_t a_size) {
        const size_t maxPow(_getMaxPow());
        size_t bufferSize;
        if (a_size <= StaticSize) {
          bufferSize = StaticSize;
        } else if (maxPow != 0 && a_size >= maxPow) {
          bufferSize = _getOwerPowSize(a_size);
        } else {
          bufferSize = _getPowSize(a_size);
          if (maxPow != 0 && bufferSize >= maxPow) {
            bufferSize = maxPow;
          }
        }
        return bufferSize;
      }

      static size_t _getPreviousSize(size_t a_size, bool a_forcePowMode = false) {
        const size_t maxPow(_getMaxPow());
        if (maxPow == 0 || a_size < maxPow || a_forcePowMode) {
          if (a_size >= OffsetSize) {
            a_size -= OffsetSize;
          }
          if (StepModuleFrac) {
            const long double stepMode(_getStepMode());
            return std::max((size_t)StaticSize, (size_t)((long double)(a_size > StepSize ? a_size - StepSize : 0) / stepMode) + (size_t)OffsetSize);
          } else {
            return std::max((size_t)StaticSize, (a_size > StepSize ? a_size - StepSize : 0) / (size_t)(StepModuleInt ? StepModuleInt : 1) + (size_t)OffsetSize);
          }
        } else {
          const size_t maxPowStep(_getMaxPowStep());
          size_t step = _getOwerPowStep(a_size);
          if (!step) {
            return _getPowSize(a_size - 1, 0);
          }
          return maxPow + (step - 1) * maxPowStep;
        }
      }

      static size_t _getNextSize(size_t a_size) {
        const size_t maxPow(_getMaxPow());
        if (maxPow == 0 || a_size < maxPow) {
          size_t result;
          if (a_size >= OffsetSize) {
            a_size -= OffsetSize;
          }
          if (!StepModuleFrac) {
            result = a_size * StepModuleInt + StepSize + OffsetSize;
          } else {
            const long double stepMode(_getStepMode());
            result = (size_t)((long double)a_size * stepMode + StepSize + OffsetSize);
          }
          return (maxPow == 0 || result <= maxPow) ? result : _getBufferSize(a_size+1);
        } else {
          return _getBufferSize(a_size+1);
        }
      }

      static size_t _getMaxPowImpl() {
        return MaxPow ? _getPowSize(MaxPow) : 0;
      }

      static size_t _getMaxPow() {
        static const size_t result(_getMaxPowImpl());
        return result;
      }

      static double _getStepModeImpl() {
        double pow = std::pow((double)StepModuleFrac, -std::ceil(std::log10((double)StepModuleFrac)));
        return (double)StepModuleInt + (double)StepModuleFrac * pow;
      }

      static double _getStepMode() {
        static const double result(_getStepModeImpl());
        return result;
      }

      static size_t _getMaxPowStepImpl() {
        const size_t maxPow(_getMaxPow());
        size_t curSize = _getPowSize(maxPow);
        size_t prevSize = _getPreviousSize(curSize, true);
        return curSize - prevSize;
      }

      static size_t _getMaxPowStep() {
        static const size_t result(_getMaxPowStepImpl());
        return result;
      }

      static size_t _getPowSize(size_t a_size, size_t a_powInc = 1) {
        if (StepModuleInt == 1 && StepModuleFrac == 0) {
          if (a_size >= OffsetSize){
            a_size -= OffsetSize;
          }
          size_t step = a_size / StepSize;
          if (a_size % StepSize){
            ++step;
          }
          if (step && a_powInc == 0){
            step -= 1;
          }
          return step * StepSize + OffsetSize;
        } else {
          if (a_powInc) {
            if (_pps.is(a_size)) {
              return _pps.get(a_size);
            }
          } else  {
            if (_pps.isp(a_size)) {
              return _pps.getp(a_size);
            }
          }
          if (a_size >= OffsetSize){
            a_size -= OffsetSize;
          }
          const long double stepMode(_getStepMode());
          long double val = ( (long double)a_size * stepMode + (long double)StepSize - (long double)a_size ) / (long double)StepSize;
          long double log = std::log(val) / std::log(stepMode);
          long double pow = std::ceil(log - 1);
          long double powStepMode = std::pow(stepMode, pow + a_powInc);
          long double bufferSize = (long double)(StepSize * (powStepMode - 1) ) / (long double)(stepMode - 1);
          return (size_t)bufferSize + OffsetSize;
        }
      }

      static size_t _getOwerPowStep(size_t a_size){
        const size_t maxPow(_getMaxPow());
        const size_t maxPowStep(_getMaxPowStep());
        size_t diff = a_size - maxPow;
        size_t step = diff / maxPowStep;
        if (diff % maxPowStep) {
          ++step;
        }
        return step;
      }

      static size_t _getOwerPowSize(size_t a_size){
        const size_t maxPow(_getMaxPow());
        const size_t maxPowStep(_getMaxPowStep());
        size_t step = _getOwerPowStep(a_size);
        return maxPow + step * maxPowStep;
      }

      char                    _adata[StaticSize * sizeof(Ty)];
      Ty*                     _pdata;
      std::unique_ptr<char[]> _mdata;
      size_t                  _sdata;
      size_t                  _cdata;
      static PrebuildPowSizes _pps;
  };

  template<typename Ty, unsigned int StaticSize, unsigned int OffsetSize, unsigned int StepSize, unsigned int StepModuleInt, unsigned int StepModuleFrac, unsigned int MaxPow>
  typename StaticVector<Ty, StaticSize, OffsetSize, StepSize, StepModuleInt, StepModuleFrac, MaxPow>::PrebuildPowSizes StaticVector<Ty, StaticSize, OffsetSize, StepSize, StepModuleInt, StepModuleFrac, MaxPow>::_pps;

} // fcf namespace


#endif // #ifndef ___FCF_BASIS__BITS__STATIC_VECTOR__STATIC_VECTOR_HPP___
