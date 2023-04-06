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

import jwt


class TokenValidation():

    def get_token(self, header):
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
        if 'scope' in self.token_decoded is None:
            print("ERROR Decoding Bearer token: No scope found", flush=True)
            return False
        scopes_token = self.token_decoded['scope']
        if role in scopes_token:
            return True
        else:
            return False
