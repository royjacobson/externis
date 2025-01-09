/**
 * Copyright (C) 2022 Roy Jacobson
 * https://github.com/royjacobson/externis
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

#include "externis.h"
#include <plugin-version.h>
#include <sys/types.h>
#include <unistd.h>

namespace externis {

constexpr int MINIMUM_EVENT_LENGTH_NS = 1000000; // 1ms

namespace {

json::object *output_json;
json::array *output_events_list;
static std::FILE *trace_file;

const char *category_string(EventCategory cat) {
  static const char *strings[10] = {
      "TU",          "PREPROCESS", "FUNCTION",        "STRUCT",  "NAMESPACE",
      "GIMPLE_PASS", "RTL_PASS",   "SIMPLE_IPA_PASS", "IPA_PAS", "UNKNOWN"};
  return strings[(int)cat];
}

json::object *new_event(const TraceEvent &event, int pid, int tid, TimeStamp ts,
                        const char *phase, int this_uid) {
  json::object *json_event = new json::object;
  json_event->set("name", new json::string(event.name));
  json_event->set("ph", new json::string(phase));
  json_event->set("cat", new json::string(category_string(event.category)));
  json_event->set("ts", new json::integer_number(ts));
  json_event->set("pid", new json::integer_number(pid));
  json_event->set("tid", new json::integer_number(tid));
  json::object *args = new json::object();
  args->set("UID", new json::integer_number(this_uid));
  if (event.args) {
    for (auto &[key, value] : *event.args) {
      args->set(key.data(), new json::string(value.data()));
    }
  }
  json_event->set("args", args);
  return json_event;
}
} // namespace

void set_output_file(FILE *file) {
  trace_file = file;
  output_json = new json::object();
  output_json->set("displayTimeUnit", new json::string("ns"));
  output_json->set("beginningOfTime",
                   new json::integer_number(
                       std::chrono::duration_cast<std::chrono::nanoseconds>(
                           COMPILATION_START.time_since_epoch())
                           .count()));
  output_json->set("traceEvents", new json::array());

  output_events_list = (json::array *)output_json->get("traceEvents");
}

void add_event(const TraceEvent &event) {
  static int pid = getpid();
  static int tid = 0;
  static int UID = 0;
  if ((event.ts.end - event.ts.start) < MINIMUM_EVENT_LENGTH_NS) {
    return;
  }
  int this_uid = UID++;
  output_events_list->append(
      new_event(event, pid, tid, event.ts.start, "B", this_uid));
  output_events_list->append(
      new_event(event, pid, tid, event.ts.end, "E", this_uid));
}

void write_all_events() {
  add_event(
      TraceEvent{"TU", EventCategory::TU, {0, ns_from_start()}, std::nullopt});
  write_preprocessing_events();
  write_opt_pass_events();
  write_all_functions();
  write_all_scopes();

#if GCCPLUGIN_VERSION_MAJOR >= 14
  output_json->dump(trace_file, /*formatted=*/false);
#else
  output_json->dump(trace_file);
#endif
  fclose(trace_file);

  output_events_list = nullptr;
  delete output_json;
}

} // namespace externis
