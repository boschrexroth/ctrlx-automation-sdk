#!/usr/bin/env python3

# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import os
import sys
import time
from typing import List
import logging
from cysystemd import journal

import ctrlxdatalayer
from ctrlxdatalayer.bulk import Response, BulkReadRequest, BulkWriteRequest, BulkCreateRequest
from ctrlxdatalayer.variant import Result, Variant
from comm.datalayer import Metadata

from helper.ctrlx_datalayer_helper import get_client

log = logging.getLogger('sdk-py-client-bulk')
if 'SNAP' in os.environ:
    log.addHandler(journal.JournaldLogHandler())
else:
    log.addHandler(logging.StreamHandler(sys.stdout))
log.setLevel(logging.DEBUG)

def cb_read_async(responses: List[Response], userdata: ctrlxdatalayer.clib.userData_c_void_p):
    """cb_read_async
    """
    for response in responses:
        if response.get_result() == Result.OK:
            dt = response.get_datetime()
            addr = response.get_address()
            data = response.get_data()
            log.debug('Bulk read: {a} {b} {c}'.format(a=dt, b=addr, c=data.get_float64()))


def main():
    """main
    """
    log.debug("=================================================================")
    log.debug("sdk-py-client-bulk - A ctrlX Data Layer Client App in Python")
    log.debug("=================================================================")

    with ctrlxdatalayer.system.System("") as datalayer_system:
        datalayer_system.start(False)

        datalayer_client, datalayer_client_connection_string = get_client(
            datalayer_system, ip="10.0.2.2", ssl_port=8443)
        if datalayer_client is None:
            log.error("Connecting '{c}' failed".format(c=datalayer_client_connection_string))
            sys.exit(1)

        with datalayer_client:  # datalayer_client is closed automatically when leaving with block

            if datalayer_client.is_connected() is False:
                log.error("ctrlX Data Layer is NOT connected:" + datalayer_client_connection_string)
                sys.exit(2)

            # Bulk read sync
            __bulk_read(datalayer_client)

            # Bulk write sync
            __bulk_write(datalayer_client)

            # Bulk browse sync
            __bulk_browse(datalayer_client)

            # Bulk metadata sync
            __bulk_metadata(datalayer_client)

            # Bulk create sync
            __bulk_create(datalayer_client)

            # Bulk delete sync
            __bulk_delete(datalayer_client)

            log.debug("-----------------------------------------------------------------")
            log.debug("Running async operations")
            log.debug("-----------------------------------------------------------------")

            # Attention: Keep bulk instance during async operation
            bulk = datalayer_client.create_bulk()
            req = [BulkReadRequest('framework/metrics/system/cpu-utilisation-percent'), 
                   BulkReadRequest('framework/metrics/system/memavailable-mb')]
            while datalayer_client.is_connected():

                # Bulk read async
                result = bulk.read(req, cb_read_async)

                time.sleep(5.0)

            log.error("ctrlX Data Layer is NOT connected")

        # Attention: Doesn't return if any provider or client instance is still running
        stop_ok = datalayer_system.stop(False)
        log.debug("System Stop {a}".format(a=stop_ok))

def __bulk_delete(client):
    """__bulk_delete
    """
    bulk = client.create_bulk()
    with bulk:
        result = bulk.delete(['sdk-cpp-alldata/dynamic/py-created/int8', 'sdk-cpp-alldata/dynamic/py-created/string'])
        if result != Result.OK:
            log.debug('Bulk delete error: {a}'.format(result))
            return
        for response in bulk.get_response():
            log.debug('Bulk delete: {a} {b}'.format(a=response.get_address(), b=response.get_result()))

def __bulk_create(client):
    """__bulk_create
    """
    bulk = client.create_bulk()
    with bulk:
        with Variant() as data:
            data.set_int8(-127)
            req = [BulkCreateRequest('sdk-cpp-alldata/dynamic/py-created/int8', data)]
            data.set_string('Created by bulk create')
            req.append(BulkCreateRequest('sdk-cpp-alldata/dynamic/py-created/int8', data))
            result = bulk.create(req)
            if result != Result.OK:
                log.debug('Bulk create error: {a}'.format(result))
                return
            for response in bulk.get_response():
                log.debug('Bulk create: {a} {b}'.format(a=response.get_address(), b=response.get_result()))

def __bulk_metadata(client):
    """__bulk_metadata
    """
    bulk = client.create_bulk()
    with bulk:
        result = bulk.metadata(['framework/metrics/system/cpu-utilisation-percent','framework/metrics/system/memavailable-mb'])
        if result != Result.OK:
            log.debug('Bulk metadata error: {a}'.format(result))
            return
        for response in bulk.get_response():
            log.debug('Bulk metadata: {a} {b}'.format(a=response.get_address(), b=response.get_result()))
            if response.get_result() != Result.OK:
                log.debug('Bulk metadata response error: {a}'.format(response.get_result()))
                continue
            metadata = Metadata.Metadata.GetRootAsMetadata(
                        response.get_data().get_flatbuffers(), 0)
            allowedoperations = metadata.Operations()
            log.debug("\tallowedOperations read={a} write={b} create={c} delete={d}"
                        .format(a=allowedoperations.Read(),b=allowedoperations.Write(),c=allowedoperations.Create(),d=allowedoperations.Delete()))

def __bulk_browse(client):
    """__bulk_browse
    """
    bulk = client.create_bulk()
    with bulk:
        result = bulk.browse(['framework/metrics/system', 'framework/metrics/process'])
        if result != Result.OK:
            log.debug('Bulk browse error: {a}'.format(result))
            return
        for response in bulk.get_response():
            log.debug('Bulk browse: {a} {b}'.format(a=response.get_address(), b=response.get_result()))
            if response.get_result() != Result.OK:
                log.debug('Bulk browse response error: {a}'.format(response.get_result()))
                continue
            for node in response.get_data().get_array_string():
                log.debug('\t{a}'.format(a=node))

def __bulk_write(client):
    """__bulk_write
    """
    bulk = client.create_bulk()
    with bulk:
        req = []
        data = Variant()
        data.set_bool8(False)
        req.extend([BulkWriteRequest('sdk-py-provider-alldata/dynamic/bool8', data),
                BulkWriteRequest('sdk-cpp-alldata/dynamic/bool8', data)])

        dataf = Variant()
        dataf.set_float32(1.2345)
        req.extend([BulkWriteRequest('sdk-py-provider-alldata/dynamic/float32', dataf),
                    BulkWriteRequest('sdk-cpp-alldata/dynamic/float32', dataf)])

        datai = Variant()
        datai.set_int8(123)
        req.extend([BulkWriteRequest('sdk-py-provider-alldata/dynamic/int8', datai),
                    BulkWriteRequest('sdk-cpp-alldata/dynamic/int8', datai)])

        datas = Variant()
        datas.set_string('Changed by bulk write :-)')
        req.extend([BulkWriteRequest('sdk-py-provider-alldata/dynamic/string', datas),
                    BulkWriteRequest('sdk-cpp-alldata/dynamic/string', datas)])

        result = bulk.write(req)
        data.close()
        dataf.close()
        datai.close()
        datas.close()
        if result != Result.OK:
            log.debug('Bulk write error: {a}'.format(result))
            return
        for response in bulk.get_response():
            log.debug('Bulk write: {a} {b}'.format(a=response.get_address(), b=response.get_result()))

def __bulk_read(client):
    """__bulk_read
    """
    bulk = client.create_bulk()
    with bulk:
        req = [BulkReadRequest(
            'framework/metrics/system/cpu-utilisation-percent'),
            BulkReadRequest(
            'framework/metrics/system/memavailable-mb')]
        result = bulk.read(req)
        if result != Result.OK:
            log.debug('Bulk read error: {a}'.format(result))
            return
        for response in bulk.get_response():
            if response.get_result() != Result.OK:
                log.debug('Bulk read response error: {a}'.format(response.get_result()))
                continue
            dt = response.get_datetime()
            addr = response.get_address()
            data = response.get_data()
            log.debug('Bulk read: {a} {b} {c}'.format(a=dt, b=addr, c=data.get_float64()))

if __name__ == '__main__':
    main()
