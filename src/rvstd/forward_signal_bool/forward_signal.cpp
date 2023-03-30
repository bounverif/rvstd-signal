//g++ -std=c++17 -c -O3 forward_signal.cpp -o forward_signal.o
//ar cr libforward_signal.a forward_signal.o

#include "forward_signal_bool/forward_signal.hpp"
#include "forward_signal_bool/boolean_operations.hpp"

using namespace rvstd::forward_signal_bool;

forward_signal::forward_signal( std::initializer_list< std::pair< TypeT, TypeT > > common_, TypeV init_ )
   : init( init_ )
{
   for( std::pair< TypeT, TypeT > p : common_ ) {
      append( p.first, !init );
      append( p.second, init );
   }
}

forward_signal::forward_signal( std::initializer_list< std::pair< int, int > > common_, TypeV init_ )
   : init( init_ )
{
   for( const std::pair< int, int >& p : common_ ) {
      TypeT t_type_val1 = TypeT( p.first );
      append( t_type_val1, !init );
      TypeT t_type_val2 = TypeT( p.second );
      append( t_type_val2, init );
   }
}

void forward_signal::reserve( size_type n )
{
   data.reserve( n );
}

auto forward_signal::capacity() const noexcept -> size_type
{
   return data.capacity();
}

auto forward_signal::size() const noexcept -> size_type
{
   return data.size();
}

auto forward_signal::empty() const noexcept -> bool
{
   return data.empty();
}

void forward_signal::append( const TypeT& pos, const TypeV& value )
{
   if( data.empty() && value == init ) {
      return;
   }
   if( data.empty() && value != init ) {
      data.emplace_back( std::make_pair( pos, value ) );
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
      if( ( data.empty() && value != init ) || ( !data.empty() && value != data.back().second ) ) {
         data.emplace_back( std::make_pair( pos, value ) );
      }
      return;
   }
   data.emplace_back( std::make_pair( pos, value ) );
}

auto forward_signal::at( const TypeT& pos ) -> const TypeV&
{
   if( data.empty() ) {
      return init;
   }
   typename std::vector< std::pair< TypeT, TypeV >, AllocatorT >::iterator up;
   std::pair< TypeT, TypeV > target( pos, 0 );
   up = std::lower_bound( data.begin(), data.end(), target, []( const std::pair< TypeT, TypeV >& a, const std::pair< TypeT, TypeV >& b ) { return a.first <= b.first; } );

   int index = std::distance( data.begin(), up ) - 1;
   return index < 0 ? init : data[ index ].second;
}

auto forward_signal::get_init() const -> const TypeV&
{
   return init;
}

auto forward_signal::get_data_vector() const -> const std::vector< std::pair< TypeT, TypeV >, AllocatorT >&
{
   return data;
}

void forward_signal::set_data_vector( const std::vector< std::pair< TypeT, TypeV >, AllocatorT >& data_ )
{
   data = data_;
}

auto forward_signal::to_string() -> const std::string
{
   std::string ret;
   ret = "init = ";
   ret += std::to_string( init );
   ret += " data ={";
   for( auto const& elem : data ) {
      ret += " ( ";
      ret += std::to_string( elem.first.count() );
      ret += ", ";
      ret += std::to_string( elem.second );
      ret += " )";
   }
   ret += " }";
   return ret;
}

auto forward_signal::operator+( const forward_signal& other ) const -> forward_signal
{
   return set_union( *this, other );
}

auto forward_signal::operator&( const forward_signal& other ) const -> forward_signal
{
   return set_intersection( *this, other );
}

auto forward_signal::operator-( const forward_signal& other ) const -> forward_signal
{
   return set_difference( *this, other );
}

auto forward_signal::operator not() const -> forward_signal
{
   return set_complement( *this );
}
