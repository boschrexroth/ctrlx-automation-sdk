import jwt
import json

def checkPermissions (token : str, scopes_list):

    try:
        token_decoded=jwt.decode(token,options={"verify_signature": False})
        
        if 'scope' in token_decoded is None:
            print("ERROR Decoding bearertoken: No scope found")
            return None
        scopes_token = token_decoded['scope']
        scopes_validated_json = {}
        for scope in scopes_list:
            if scope in scopes_token:
                scopes_validated_json[str(scope)]= True
            else:
                scopes_validated_json[str(scope)]= False
            
        return scopes_validated_json
    except jwt.DecodeError:
        print("ERROR Decoding bearertoken")
        return None
    
