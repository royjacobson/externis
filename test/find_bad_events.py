#!/usr/bin/env python3
import argparse
import json


def find_bad_events(data):
    """
    Ambitiously slow.
    This function finds events that intersect but aren't strictly contained
    in each other.
    This is usually a bug.
    """
    id_to_start = {event['args']['UID'] : event for event in data['traceEvents'] if event['ph'] == 'B'}
    id_to_end = {event['args']['UID'] : event for event in data['traceEvents'] if event['ph'] == 'E'}
    for id_1 in id_to_start:
        for id_2 in id_to_start:
            if id_1 == id_2:
                continue
            start_1, start_2 = id_to_start[id_1]['ts'], id_to_start[id_2]['ts']
            end_1, end_2 = id_to_end[id_1]['ts'], id_to_end[id_2]['ts']
            if end_1 > start_2 > start_1 and not (end_1 > end_2 > start_1):
                print(f"""
Event 1: {id_to_start[id_1]['name']}
Event 2: {id_to_start[id_2]['name']}
Order is START1    START2    END1      END2     
         {start_1} +{start_2 - start_1:< 8} +{end_1 - start_2:< 8} +{end_2 - end_1:< 8}""")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('traces', nargs='+')
    args = parser.parse_args()
    for file in args.traces:
        print(f"Processing {file}...")
        find_bad_events(json.load(open(file)))
