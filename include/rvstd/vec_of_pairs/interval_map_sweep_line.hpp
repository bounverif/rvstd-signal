#ifndef INTERVAL_MAP_RVSTD
#define INTERVAL_MAP_RVSTD

#include <algorithm>
#include <chrono>
#include <climits>
#include <cstdlib>
#include <new>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>
#include <iostream>


/*
* ({ "{ {_ , _}, _}"; "{ {_ , _}, _}", ... })
*/
namespace rvstd
{
   namespace vec_of_pairs_sweep_line
   {
      template< class TypeT = double, class TypeV = int,class AllocatorT = std::allocator< TypeT > >
      class interval_map
      {
      private:
         std::vector<std::pair< TypeT, TypeV > > data;// Data buffer
         TypeV init;
         TypeV base = 0;


      public:
         using size_type = std::size_t;
         
         interval_map( std::initializer_list< std::pair< std::pair< TypeT, TypeT >, TypeV > > common_, TypeV init_ = 0 )
         {
            init = init_;
            for( std::pair< std::pair< TypeT, TypeT >, TypeV > p : common_ ) {
               std::pair< TypeT, TypeT > interval_ = p.first;
               TypeV value_ = p.second;

               if( interval_.first > interval_.second ) {
                  throw std::invalid_argument( "invalid interval" );
               }
               if( interval_.first == interval_.second ) {
                  continue;
               }
               if( !data.empty() && interval_.first < data.back().first ) {
                  throw std::invalid_argument( "invalid interval" );
               }
               if( !data.empty() && interval_.first == data.back().first && value_ == data[data.size()-2].second ) {
                  data.pop_back();
                  data.push_back( std::make_pair(interval_.second, base) );
                  continue;
               }
               if( !data.empty() && interval_.first == data.back().first ){
                  data.back().second = value_;
                  data.push_back(std::make_pair(interval_.second,base));
                  continue;
               }
               if( data.empty() &&  value_ == init_ ){
                  if( init_ != base){
                  data.push_back( std::make_pair(interval_.second,base) );
                  }
                  continue;
               }
               
               data.push_back( std::make_pair(interval_.first,value_));
               data.push_back( std::make_pair(interval_.second,base) );
            }
         }
         /*
         interval_map( bool_and_vec_sweep_line::interval_set<TypeT> x, std::vector<TypeV> y, TypeV init_ = 0 )
         {
            if (x.size() != y.size()){
               throw std::invalid_argument( "size of vectors should be equal" );
            }
            init = init_;

            for(int i = 0; i < x.size(); i++){
               std::cout << i << ":  "<< x.get_data_vector()[i] << ", " << y[i] << "\n";
               append(x.get_data_vector()[i],y[i] );
            }
         }*/

         explicit interval_map( TypeV init_ = 0 )
            : init( init_ ){};

         ~interval_map() = default;
         interval_map( interval_map&& ) noexcept = default;
         interval_map& operator=( interval_map&& ) noexcept = default;
         interval_map( const interval_map& ) = default;
         interval_map& operator=( const interval_map& ) = default;

         void reserve( size_type n )
         {
            data.reserve( n );
         };
         size_type capacity() const noexcept
         {
            return data.capacity();
         };
         size_type size() const noexcept
         {
            return data.size();
         };

         bool empty() const noexcept
         {
            return data.empty();
         };

         std::string get_allocator_type() const noexcept
         {
            return ( std::string )( typeid( AllocatorT ).name );
         };  // returns the type of allocator.

         bool contains( const std::pair< TypeT, TypeT >& ) noexcept;

         std::pair< TypeT, TypeT > find( const TypeT ) noexcept;      // finds and returns specific interval.
         void clear() noexcept;                                       // Remove completely.
         void delete_interval( int pos ) noexcept;                    // used to remove posth interval. Can be used to delete first element. Left to right order.
         void delete_interval( std::pair< TypeT, TypeT > ) noexcept;  //delete specific interval.
         
         /*
         * adds an element to end of the list
         * O(1)
         */
         void append( TypeT pos, TypeV value )
         {
            if( data.size() == 0 && value == init ) {
               return;
            }
            if( data.size() == 0 && value != init ) {
               data.push_back( std::make_pair(pos, value) );
               return;
            }

            if( pos < data.back().first ) {
               throw( " cannot add to mid of the list " );
            }

            if( value == data.back().second ) {
               return;
            }

            if( pos == data.back().first ) {
               data.pop_back();
               if( value != data.back().second ){
                  data.push_back( std::make_pair(pos, value) );
               }
               return;
            }
            data.push_back( std::make_pair(pos, value) );
         };


         /*
         * O(log(n))
         * returns value at a spesific time
         */
         TypeV at( const TypeT pos )
         {
            if( data.size() == 0) {
               return init;
            }
            typename std::vector< std::pair<TypeT, TypeV> >::iterator up;

            up = std::lower_bound( data.begin(), data.end(), std::make_pair(pos,0),
             [](const std::pair<TypeT, TypeV> & a, const std::pair<TypeT, TypeV> & b) { return a.first <= b.first; } );
            
            int index = std::distance(data.begin(),up)-1;            
            return index < 0 ? init : data[index].second;
         };



         template< typename TypeP >
         interval_map< TypeT, TypeP, AllocatorT > set_operations( const interval_map< TypeT, TypeP, AllocatorT >& other, std::function< TypeP( TypeP, TypeP ) > op )
         {
            TypeP this_val = init;
            TypeP other_val = other.get_init();

            //assign init of list constructed
            TypeP ret_bool = op( this_val, other_val );

            //create new list
            interval_map< TypeT, TypeP, AllocatorT > ret( ret_bool );

            ret.reserve( round_up_above_2_bit( data.size() + other.size() ) );

            std::vector< std::pair<TypeT, TypeP > > other_vec = other.get_data_vector();
            std::vector< std::pair<TypeT, TypeP > > ret_vec;
            int this_index = 0, other_index = 0;
            bool is_done_this = ( data.size() == 0 ), is_done_other = ( other_vec.size() == 0 );

            while( !is_done_other || !is_done_this ) {
               TypeT value;

               if( is_done_other || ( !is_done_this && data[ this_index ].first <= other_vec[ other_index ].first ) ) {
                  this_val = data[this_index].second;
                  value = data[ this_index ].first;
                  ++this_index;
               }
               else if( is_done_this || ( !is_done_other && data[ this_index ].first > other_vec[ other_index ].first ) ) {
                  other_val = other_vec[ other_index ].second;
                  value = other_vec[ other_index ].first;
                  ++other_index;
               }
               TypeP calc_val = op( this_val, other_val );
               ret.append( value, calc_val );

               is_done_this = this_index >= data.size();
               is_done_other = other_index >= other_vec.size();
            }
            return ret;
         }

         //maxs
         interval_map< TypeT, TypeV, AllocatorT > set_union( const interval_map< TypeT, TypeV, AllocatorT >& other )
         {
            std::function< TypeV( TypeV, TypeV ) > binary_op = []( TypeV x, TypeV y ) { return ( x < y ? y : x ); };
            return set_operations< TypeV >( other, binary_op );
         }
         /*
         interval_map< TypeT, TypeV, AllocatorT > set_difference( const interval_map< TypeT, TypeV, AllocatorT >& other )
         {
            std::function< TypeV( TypeV, TypeV ) > binary_op = []( TypeV x, TypeV y ) { return ( x < y ? y : x ); };
            return set_operations< TypeV >( other, binary_op );
         }*/
         interval_map< TypeT, TypeV, AllocatorT > set_intersection( const interval_map< TypeT, TypeV, AllocatorT >& other )
         {
            std::function< TypeV( TypeV, TypeV ) > binary_op = []( TypeV x, TypeV y ) { return ( x < y ? x : y ); };
            return set_operations< TypeV >( other, binary_op );
         }
         interval_map< TypeT, TypeV, AllocatorT > set_complement()
         {
            interval_map< TypeT, TypeV, AllocatorT > not_this( base - init );
            std::vector<std::pair< TypeT, TypeV > > new_data;

            for(std::pair< TypeT, TypeV > p : data){
               new_data.push_back(std::make_pair(p.first, base - p.second));
            }
            not_this.set_data_vector( new_data );
            return not_this;
         }

         interval_map< TypeT, TypeV, AllocatorT > operator+( const interval_map< TypeT, TypeV, AllocatorT >& other )
         {
            return set_union( other );
         }
         /*
         interval_map< TypeT, TypeV, AllocatorT > operator-( const interval_map< TypeT, TypeV, AllocatorT >& other )
         {
            return set_difference( other );
         }*/
         interval_map< TypeT, TypeV, AllocatorT > operator&( const interval_map< TypeT, TypeV, AllocatorT >& other )
         {
            return set_intersection( other );
         }
         interval_map< TypeT, TypeV, AllocatorT > operator not()
         {
            return set_complement();
         }

         // UTILITY
         const std::string to_string();
         const std::vector<std::pair< TypeT, TypeV > >& get_data_vector() const
         {
            return data;
         }
         TypeV get_init() const
         {
            return init;
         }
         void set_data_vector( const std::vector<std::pair< TypeT, TypeV > >& data_ )
         {
            data = data_;
         }
         unsigned int round_up_above_2_bit( unsigned int x )
         {
            x--;
            for( unsigned int i = 1; i < 16; i *= 2 ) {
               x |= x >> i;
            }
            x++;
            return x;
         }
      };
   }  // namespace vec_of_pairs_sweep_line
}  // namespace rvstd

#endif  // INTERVAL_MAP_RVSTD
