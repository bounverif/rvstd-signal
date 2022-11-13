#ifndef INTERVAL_SET_RVSTD
#define INTERVAL_SET_RVSTD

#include <algorithm>
#include <chrono>
#include <climits>
#include <cstdlib>
#include <new>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

namespace rvstd {
   namespace bool_and_vec_skip_line {
      template< class TypeT = double, class AllocatorT = std::allocator<TypeT> >
      class interval_set {
      private:
         std::vector<TypeT> data;  // Data buffer
         bool init;

      public:
         using size_type = std::size_t;

         interval_set(std::initializer_list<std::pair<TypeT, TypeT> > common_, bool init_ = false) : init(init_) {
            for (std::pair<TypeT, TypeT> p : common_) {
               if (p.first > p.second) {
                  throw std::invalid_argument("invalid interval");
               }
               if (p.first == p.second) {
                  continue;
               }
               if (!data.empty() && p.first < data.back()) {
                  throw std::invalid_argument("invalid interval");
               }
               if (!data.empty() && p.first == data.back()) {
                  data.pop_back();
                  data.push_back(p.second);
                  continue;
               }
               data.push_back(p.first);
               data.push_back(p.second);
            }
         }

         explicit interval_set( bool init_ = false )
            : init( init_ ){};

         ~interval_set() = default;

         interval_set(interval_set &&) noexcept = default;

         interval_set &operator=(interval_set &&) noexcept = default;

         interval_set(const interval_set &) = default;

         interval_set &operator=(const interval_set &) = default;

         void reserve(size_type n) {
            data.reserve(2 * n);
         };

         size_type capacity() const noexcept {
            return data.capacity() / 2;
         };

         size_type size() const noexcept {
            return data.size() / 2;
         };

         bool empty() const noexcept {
            return data.empty();
         };

         std::string get_allocator_type() const noexcept {
            return ( std::string )( typeid( AllocatorT ).name );
         };  // returns the type of allocator.

         bool contains(const std::pair<TypeT, TypeT> &) noexcept;

         std::pair<TypeT, TypeT> find(const TypeT) noexcept;      // finds and returns specific interval.
         void clear() noexcept;                                       // Remove completely.
         void delete_interval( int pos ) noexcept;                    // used to remove posth interval. Can be used to delete first element. Left to right order.
         void delete_interval(std::pair<TypeT, TypeT>) noexcept;  //delete specific interval.

         void append(TypeT num) {
            // TODO
            // Controls may inserted here.
            this->get_data_vector().push_back(num);
         };  // adds an interval to end of the list

         void pop_back() {
            this->get_data_vector().pop_back();
         }

         bool at( const TypeT pos )
         {
            if( data.size() == 0 ) {
               return init;
            }
            typename std::vector< TypeT >::iterator up;
            up = std::upper_bound( data.begin(), data.end(), pos );
            int index = up - data.begin();  //how much change
            return ( !init != !( index % 2 ) );
         };

         // UTILITY
         std::string to_string() noexcept;

         std::vector<TypeT>& get_data_vector() {
            return data;
         }

         bool get_init() {
            return init;
         }

         void set_init(bool b) {
            init = b;
         }

         // IMPLEMENTATION

         interval_set<TypeT, AllocatorT> set_union(interval_set other) {
            bool init_other = other.get_init();
            std::vector<TypeT> data_other = other.get_data_vector();
            bool init_this = this->get_init();
            std::vector<TypeT> data_this = this->get_data_vector();

            std::size_t size_this = data_this.size();
            std::size_t size_other = data_other.size();

            std::size_t cur_return = 0;
            std::size_t cur_this = 0;
            std::size_t cur_other = 0;

            bool exit_flag = false;
            bool init_this_zero_start_flag = false;
            bool init_other_zero_start_flag = false;

            unsigned int exit_status;

            if (init_this and not init_other) {
               TypeT max_val = data_other[ size_other - 1 ] + static_cast<TypeT>(1);
               if (max_val < data_this[ size_this - 1] ) {
                  max_val = data_this[ size_this - 1] + static_cast<TypeT>(1);
               }
               if (data_this[0] != 0) {
                  data_this.insert(data_this.begin(), 0);
                  data_this.push_back(max_val);
                  size_this += 2;
               } else { // (data_this[0] == 0)
                  data_this.erase(data_this.begin());
                  data_this.push_back(max_val);
                  init_this_zero_start_flag = true;
               }
            }
            else if (init_other and not init_this) {
               TypeT max_val = data_other[ size_other - 1 ] + static_cast< TypeT >( 1 );
               if( max_val < data_this[ size_this - 1 ] ) {
                  max_val = data_this[ size_this - 1 ] + static_cast< TypeT >( 1 );
               }
               if( data_other[ 0 ] != 0 ) {
                  data_other.insert( data_other.begin(), 0 );
                  data_other.push_back( max_val );
                  size_other += 2;
               }
               else {  // (data_this[0] == 0)
                  data_other.erase( data_other.begin() );
                  data_other.push_back( max_val );
                  init_other_zero_start_flag = true;
               }
            } else if (init_this and init_other) {
               TypeT max_val = data_other[ size_other - 1 ] + static_cast< TypeT >( 1 );
               if( max_val < data_this[ size_this - 1 ] ) {
                  max_val = data_this[ size_this - 1 ] + static_cast< TypeT >( 1 );
               }
               if( data_other[ 0 ] != 0 ) {
                  data_other.insert( data_other.begin(), 0 );
                  data_other.push_back( max_val );
                  size_other += 2;
               }
               else {  // (data_this[0] == 0)
                  data_other.erase( data_other.begin() );
                  data_other.push_back( max_val );
                  init_other_zero_start_flag = true;
               }
               if (data_this[0] != 0) {
                  data_this.insert(data_this.begin(), 0);
                  data_this.push_back(max_val);
                  size_this += 2;
               } else { // (data_this[0] == 0)
                  data_this.erase(data_this.begin());
                  data_this.push_back(max_val);
                  init_this_zero_start_flag = true;
               }
            }

            std::vector<TypeT> return_data(size_this + size_other);
            bool return_init = init_this or init_other;

            while (cur_this < size_this and cur_other < size_other) {
               if (data_this[cur_this] <= data_other[cur_other]) {
                  return_data[cur_return] = data_this[cur_this];
                  cur_return++;
                  cur_this++;

                  if (cur_this == size_this) { // Exit without exception.
                     break;
                  }

                  while (true) {
                     while (data_this[cur_this] > data_other[cur_other]) {  // Iterate until first one not encapsulating the second set.
                        cur_other++;

                        if (cur_other == size_other) { // Exit without exception.
                           return_data[cur_return] = data_this[cur_this];
                           cur_return++;
                           cur_this++;

                           exit_flag = true;
                           break;
                        }

                     }
                     if (exit_flag) {
                        break;
                     }
                     if (!(cur_other % 2)) {  // One value island is including the second set completely.
                        return_data[cur_return] = data_this[cur_this];
                        cur_return++;
                        cur_this++;
                        break;
                     } else {  // (cur_other % 2) // One value island is not including the second set completely.
                        while (data_this[cur_this] <= data_other[cur_other]) {
                           cur_this++;

                           if (cur_this == size_this) { // Exit without exception.
                              return_data[cur_return] = data_other[cur_other];
                              cur_return++;
                              cur_other++;

                              exit_flag = true;
                              break;
                           }

                        }
                        if (exit_flag) {
                           break;
                        }
                        if (not(cur_this %
                                2)) {  // One value island is including the second set completely.
                           return_data[cur_return] = data_other[cur_other];
                           cur_return++;
                           cur_other++;
                           break;
                        } else {  // (cur_this % 2) // One value island is not including the second set completely.
                           // Nothing to do. Iterate more.
                        }
                     }
                  }
               } else if (data_this[cur_this] > data_other[cur_other]) {
                  return_data[cur_return] = data_other[cur_other];
                  cur_return++;
                  cur_other++;

                  if (cur_other == size_other) { // Exit without exception.
                     break;
                  }

                  while (true) {
                     while (data_this[cur_this] <= data_other[cur_other]) {  // Iterate until first one not encapsulating the second set.
                        cur_this++;

                        if (cur_this == size_this) { // Exit without exception.
                           return_data[cur_return] = data_other[cur_other];
                           cur_return++;
                           cur_other++;

                           exit_flag = true;
                           break;
                        }

                     }
                     if (exit_flag) {
                        break;
                     }
                     if (!(cur_this % 2)) {  // One value island is including the second set completely.
                        return_data[cur_return] = data_other[cur_other];
                        cur_return++;
                        cur_other++;
                        break;
                     } else {  // (cur_this % 2) // One value island is not including the second set completely.
                        while (data_this[cur_this] > data_other[cur_other]) {
                           cur_other++;

                           if (cur_other == size_other) { // Exit without exception.
                              return_data[cur_return] = data_this[cur_this];
                              cur_return++;
                              cur_this++;

                              exit_flag = true;
                              break;
                           }

                        }
                        if (exit_flag) {
                           break;
                        }
                        if (not(cur_other %
                                2)) {  // One value island is including the second set completely.
                           return_data[cur_return] = data_this[cur_this];
                           cur_return++;
                           cur_this++;
                           break;
                        } else {  // (cur_other % 2) // One value island is not including the second set completely.
                           // Nothing to do. Iterate more.
                        }
                     }
                  }
               }
            }
            if (cur_this == size_this and cur_other != size_other) {
               while (cur_other < size_other) {
                  return_data[cur_return] = data_other[cur_other];
                  cur_return++;
                  cur_other++;
               }
            } else if (cur_this != size_this and cur_other == size_other) {
               while (cur_this < size_this) {
                  return_data[cur_return] = data_this[cur_this];
                  cur_return++;
                  cur_this++;
               }
            }

            if (return_init) {
               if (init_this_zero_start_flag and not init_other_zero_start_flag) {
                  if (not init_other and data_other[0] != 0) {
                     return_data.insert(return_data.begin(), static_cast<TypeT>(0));
                     cur_return++;
                  } else {
                     return_data.erase(return_data.begin());
                     cur_return--;
                  }
               }
               else if (not init_this_zero_start_flag and init_other_zero_start_flag) {
                  if (not init_this and data_this[0] != 0) {
                     return_data.insert(return_data.begin(), static_cast<TypeT>(0));
                     cur_return++;
                  } else {
                     return_data.erase(return_data.begin());
                     cur_return--;
                  }
               }
               else if (init_this_zero_start_flag and init_other_zero_start_flag) {
                  return_data.insert(return_data.begin(), static_cast<TypeT>(0));
                  cur_return++;
               } else { // (not init_this_zero_start_flag and not init_other_zero_start_flag)
                  return_data.erase(return_data.begin());
                  cur_return--;
               }
               return_data.erase(return_data.begin() + cur_return - 1);
               cur_return--;
            }


            return_data.resize(cur_return);
            interval_set<TypeT, AllocatorT> ret_interval_set(return_init);
            ret_interval_set.reserve(cur_return);
            TypeT prev;
            for (auto i : return_data) {
               if (prev == i) {
                  ret_interval_set.pop_back();
                  continue;
               }
               ret_interval_set.append(i);
               prev = i;
            }

            return ret_interval_set;

         }

         interval_set<TypeT, AllocatorT> set_difference(interval_set other) {
            auto ret_val = this->set_intersection(other.set_complement());
            return ret_val;
         }

         interval_set<TypeT, AllocatorT> set_intersection(interval_set other) {
            interval_set<TypeT, AllocatorT> ret_val(this->set_complement());
            ret_val.get_data_vector() = ret_val.set_union(other.set_complement()).get_data_vector();
            ret_val.set_init(ret_val.set_complement().get_init());
            return ret_val;
         }

         interval_set<TypeT, AllocatorT> set_complement() {
            interval_set<TypeT, AllocatorT> ret_interval_set(!init);
            ret_interval_set.get_data_vector() = this->get_data_vector();
            return ret_interval_set;
         }

         interval_set<TypeT, AllocatorT> operator+(interval_set<TypeT, AllocatorT> other) {
            return this->set_union(other);
         }

         interval_set<TypeT, AllocatorT> operator-(interval_set<TypeT, AllocatorT> other) {
            return this->set_difference(other);
         }

         interval_set<TypeT, AllocatorT> operator&(interval_set<TypeT, AllocatorT> other) {
            return this->set_intersection(other);
         }

         interval_set<TypeT, AllocatorT> operator not() {
            return this->set_complement();
         }

      };
   }  // namespace bool_and_vec_skip_line
}  // namespace rvstd

#endif  // INTERVAL_SET_RVSTD
 
