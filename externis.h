/**
 * Copyright (C) 2022 Roy Jacobson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <gcc-plugin.h>

#include <chrono>
#include <cpplib.h>
#include <cstdio>
#include <json.h>
#include <optional>
#include <pretty-print.h>
#include <string>
#include <unordered_map>

namespace externis {

// This is an alias so it's easily replacable with something else (like
// absl::flat_hash_map) if needed. Not doing it by default because I don't want
// extra dependencies.
template <class Key, class Value> using map_t = std::unordered_map<Key, Value>;

using clock_t = std::chrono::high_resolution_clock;
using time_point_t = std::chrono::time_point<clock_t>;
extern time_point_t COMPILATION_START;
using TimeStamp = int64_t;

struct TimeSpan {
  int64_t start;
  int64_t end;
};

inline TimeStamp ns_from_start() {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(clock_t::now() -
                                                              COMPILATION_START)
      .count();
}

enum EventCategory {
  TU,
  PREPROCESS,
  FUNCTION,
  STRUCT,
  NAMESPACE,
  // Pass categories
  GIMPLE_PASS,
  RTL_PASS,
  SIMPLE_IPA_PASS,
  IPA_PASS,
  //
  UNKNOWN
};

struct TraceEvent {
  const char *name;
  EventCategory category;
  TimeSpan ts;
  std::optional<map_t<std::string, std::string>> args;
};

void start_preprocess_file(const char *file_name, cpp_reader *pfile);
void end_preprocess_file();
void finish_pp_stage();
void write_preprocessing_events();

void start_opt_pass(const opt_pass *pass);
void write_opt_pass_events();

void set_output_file(FILE* file);
void add_event(const TraceEvent &event);
void write_all_events();
void write_event(const TraceEvent &, bool);

struct FinishedFunction {
  void *decl;
  const char *name;
  const char *file_name;
  const char *scope_name;
  EventCategory scope_type;
};
void end_parse_function(FinishedFunction);
void write_all_scopes();
void write_all_functions();

} // namespace externis