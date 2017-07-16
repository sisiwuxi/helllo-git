#http://python.itcarlow.ie/ed2/ch05/webapp/
from http.server import HTTPServer, CGIHTTPRequestHandler

port = 8080
httpd = HTTPServer(('',port),CGIHTTPRequestHandler)
print("Starting simple_httpd on port: " + str(httpd.server_port))
httpd.serve_forever()

