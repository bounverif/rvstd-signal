#ifndef CSV_REPORTER
#define CSV_REPORTER

#include <catch2/catch_test_case_info.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>
#include <catch2/reporters/catch_reporter_streaming_base.hpp>

#include <fstream>
#include <iostream>

std::ofstream out;
Catch::StringRef test_name;
Catch::StringRef case_name;

//benchmark_name -> (case_name -> (test_name -> "mean,sigma") )
std::map< std::string, std::map< Catch::StringRef, std::map< Catch::StringRef, std::string > > > results;

class CSVReporter : public Catch::StreamingReporterBase
{
public:
   using StreamingReporterBase::StreamingReporterBase;

   static std::string getDescription()
   {
      return "Reporter for testing csv file for rvstd benchmarks";
   }
   void testRunStarting( Catch::TestRunInfo const& testInfo ) override
   {
      test_name = testInfo.name;
      out.open( test_name + ".csv" );
   }

   void testCaseStarting( Catch::TestCaseInfo const& caseInfo ) override
   {
      case_name = caseInfo.name;
   }

   void testCaseEnded( Catch::TestCaseStats const& testCaseStats ) override {}

   void benchmarkStarting( Catch::BenchmarkInfo const& benchmarkInfo ) override
   {
      std::cout << "tests starts for rvstd\n";
   }

   void benchmarkEnded( Catch::BenchmarkStats<> const& benchmarkStats ) override
   {
      std::string mean_std = std::to_string( benchmarkStats.mean.point.count() ) + "," + std::to_string( benchmarkStats.standardDeviation.point.count() );

      //benchmark_name -> (case_name -> (test_name -> "mean,sigma") )
      std::string bench_name = benchmarkStats.info.name.c_str();
      results[ bench_name ][ case_name ][ test_name ] = mean_std;
   }

   void testRunEnded( Catch::TestRunStats const& testRunStats ) override
   {
      //benchmark_name -> (case_name -> (test_name -> "mean,sigma") )
      out << test_name << "\n";
      for( auto a : results ) {
         std::string benchmark_name = a.first;
         out << benchmark_name << ",mu,sigma\n";
         for( auto b : a.second ) {
            Catch::StringRef case_name = b.first;
            std::string values;
            for( auto c : b.second ) {
               std::string mean_std = c.second;
               values += c.second;
            }
            out << case_name << "," << values << "\n";
         }
      }
      out.close();
   }

   //~CSVReporter(){   }
};

CATCH_REGISTER_REPORTER( "CSV", CSVReporter )

#endif  // CSV_REPORTER