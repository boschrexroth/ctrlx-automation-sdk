# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import jwt


class TokenValidation():
    """TokenValidation
    """
    def get_token(self, header):
        """get_token
        """
        self.header = header
        # Check token from request header
        if 'Authorization' in self.header:
            auth_header = str(self.header['Authorization'])
            if len(auth_header) >= 1:
                bearer_token = auth_header.split()
                if len(bearer_token) == 2:
                    self.token = bearer_token[1]
                    try:
                        self.token_decoded = jwt.decode(self.token, options={"verify_signature": False})
                        return True, self.token, self.token_decoded
                    except jwt.DecodeError:
                        print("ERROR Decoding bearertoken", flush=True)
                        return False, self.token, None
        return False, None, None

    def is_authorized(self, role):
        """is_authorized
        """
        if 'scope' in self.token_decoded is None:
            print("ERROR Decoding Bearer token: No scope found", flush=True)
            return False
        scopes_token = self.token_decoded['scope']
        if role in scopes_token:
            return True
        else:
            return False
