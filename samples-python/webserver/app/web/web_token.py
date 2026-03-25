# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import jwt
import logging

log = logging.getLogger()


def check_permissions(token: str, scopes_list):
    """def check_permissions(token: str, scopes_list):

    Args:
        token (str): token
        scopes_list (list): scope list

    """
    try:
        token_decoded = jwt.decode(token, options={"verify_signature": False})

        if 'scope' in token_decoded is None:
            log.error("ERROR Decoding Bearer token: No scope found")
            return None

        scopes_token = token_decoded['scope']
        scopes_validated_json = {}
        for scope in scopes_list:
            scopes_validated_json[str(scope)] = scope in scopes_token

        return scopes_validated_json

    except jwt.DecodeError:
        log.error("ERROR decoding Bearer token: %s", token)
        return None
