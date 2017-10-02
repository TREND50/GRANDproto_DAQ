#!/usr/bin/env python

from . native import *



def print_file_header(fh):
    print("------file header------")
    print("length={0}".format(fh.length()))
    print("runnr={0}".format(fh.runnr()))
    print("run_mode={0}".format(fh.run_mode()))
    print("serial={0}".format(fh.serial()))
    print("first event={0}".format(fh.first_event()))
    print("first event sec={0}".format(fh.first_event_sec()))
    print("last event={0}".format(fh.last_event()))
    print("last event sec={0}".format(fh.last_event_sec()))
    print("------additional header------")
    print("additional header={0}".format(list(fh.additional_header())))


def print_event_header(eh):
    print("------event header------")
    print("header_length={0}".format(eh.header_length))
    print("runnr={0}".format(eh.runnr))
    print("eventnr={0}".format(eh.eventnr))
    print("t3eventnr={0}".format(eh.t3eventnr))
    print("first local station={0}".format(eh.first_ls))
    print("event_sec={0}".format(eh.event_sec))
    print("event_nsec={0}".format(eh.event_nsec))
    print("event_type={0}".format(eh.event_type))
    print("event_vers={0}".format(eh.event_vers))
    print("ad1={0}".format(eh.ad1))
    print("ad2={0}".format(eh.ad2))
    print("ls_cnt={0}".format(eh.ls_cnt))

def print_ls_header(lsh):
    print("------local station header------")
    print("length={0}".format(lsh.length))
    print("event_nr={0}".format(lsh.event_nr))
    print("ls_id={0}".format(lsh.ls_id))
    print("header_length={0}".format(lsh.header_length))
    print("GPSseconds={0}".format(lsh.GPSseconds))
    print("GPSnanoseconds={0}".format(lsh.GPSnanoseconds))
    print("trigger_flag={0}".format(lsh.trigger_flag))
    print("sampling_freq={0}".format(lsh.sampling_freq))
    print("channel_mask={0}".format(lsh.channel_mask))
    print("ADC_resolution={0}".format(lsh.ADC_resolution))
    print("trace_length={0}".format(lsh.trace_length))
    print("version={0}".format(lsh.version))


def print_local_station(ls):
    print_ls_header(ls.header)
    print("------local station header data------")
    print(list(ls.header_data))
    print("------ADC Buffer------")
    print(list(ls.adc_buffer))


def print_local_station_list(lsl):
    for i in range(0,len(lsl)):
        print("------ls {0}------".format(i))
        print_local_station(lsl[i])


def print_event(ev):
    print_event_header(ev.header)
    print_local_station_list(ev.local_station_list)


def print_event_list(el):
    for i in range(len(el)):
        print("------event {0}------".format(i))
        print_event(el[i])


def print_event_file(ef):
    print("================")
    print_file_header(ef.header)
    print_event_list(ef.event_list)
    print("================")
