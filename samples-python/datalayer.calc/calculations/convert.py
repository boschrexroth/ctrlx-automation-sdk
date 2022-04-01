# MIT License
#
# Copyright (c) 2021-2022 Bosch Rexroth AG
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

from ctrlxdatalayer.variant import Variant, VariantType


class Convert:
    def get_float64(v: Variant):
        try:
            return Convert.get_float64_internal(v)
        except Exception as e:
            print(e)

        return None

    def get_float64_internal(v: Variant):

        if v.get_type() == VariantType.FLOAT64:
            return v.get_float64()

        r = Variant()

        if v.get_type() == VariantType.FLOAT32:
            r.set_float64(v.get_float32())
            return r.get_float64()

        if v.get_type() == VariantType.INT64:
            r.set_float64(v.get_int64())
            return r.get_float64()

        if v.get_type() == VariantType.INT32:
            r.set_float64(v.get_int32())
            return r.get_float64()

        if v.get_type() == VariantType.INT16:
            r.set_float64(v.get_int16())
            return r.get_float64()

        if v.get_type() == VariantType.INT8:
            r.set_float64(v.get_int8())
            return r.get_float64()

        if v.get_type() == VariantType.UINT64:
            r.set_float64(v.get_uint64())
            return r.get_float64()

        if v.get_type() == VariantType.UINT32:
            r.set_float64(v.get_uint32())
            return r.get_float64()

        if v.get_type() == VariantType.UINT16:
            r.set_float64(v.get_uint16())
            return r.get_float64()

        if v.get_type() == VariantType.UINT8:
            r.set_float64(v.get_uint8())
            return r.get_float64()

        if v.get_type() == VariantType.BOOL8:
            if v.get_bool8():
                r.set_float64(1)
            else:
                r.set_float64(0)
            return r.get_float64()

        if v.get_type() == VariantType.STRING:
            r.set_float64(float(v.get_string()))
            return r.get_float64()

        # ARRAY ---------------------------------------------
        if v.get_type() == VariantType.ARRAY_FLOAT64:
            return v.get_array_float64()[0]

        r = Variant()

        if v.get_type() == VariantType.ARRAY_FLOAT32:
            r.set_float64(v.get_array_float32())
            return r.get_float64()

        if v.get_type() == VariantType.ARRAY_INT64:
            r.set_float64(v.get_array_int64()[0])
            return r.get_float64()

        if v.get_type() == VariantType.ARRAY_INT32:
            r.set_float64(v.get_array_int32()[0])
            return r.get_float64()

        if v.get_type() == VariantType.ARRAY_INT16:
            r.set_float64(v.get_array_int16()[0])
            return r.get_float64()

        if v.get_type() == VariantType.ARRAY_INT8:
            r.set_float64(v.get_array_int8()[0])
            return r.get_float64()

        if v.get_type() == VariantType.ARRAY_UINT64:
            r.set_float64(v.get_array_uint64()[0])
            return r.get_float64()

        if v.get_type() == VariantType.ARRAY_UINT32:
            r.set_float64(v.get_array_uint32()[0])
            return r.get_float64()

        if v.get_type() == VariantType.ARRAY_UINT16:
            r.set_float64(v.get_array_uint16()[0])
            return r.get_float64()

        if v.get_type() == VariantType.ARRAY_UINT8:
            r.set_float64(v.get_array_uint8()[0])
            return r.get_float64()

        if v.get_type() == VariantType.ARRAY_BOOL8:
            if v.get_array_bool8()[0]:
                r.set_float64(1)
            else:
                r.set_float64(0)
            return r.get_float64()

        if v.get_type() == VariantType.ARRAY_STRING:
            r.set_float64(float(v.get_array_string()[0]))
            return r.get_float64()

        return None
