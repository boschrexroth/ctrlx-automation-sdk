# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

from ctrlxdatalayer.variant import Variant, VariantType


class Convert:
    """Convert
    """
    def get_float64(v: Variant):
        """get_float64
        """
        try:
            return Convert.__get_float64_internal(v)
        except Exception as e:
            print(e, flush=True)

        return None

    def __get_float64_internal(v: Variant):
        """__get_float64_internal
        """

        if v.get_type() == VariantType.FLOAT64:
            return v.get_float64()

        with Variant() as r:

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
