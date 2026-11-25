/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

package datalayer.provider.simple

import java.io.ByteArrayOutputStream
import java.net.InetSocketAddress
import java.net.StandardProtocolFamily
import java.net.UnixDomainSocketAddress
import java.nio.ByteBuffer
import java.nio.channels.ServerSocketChannel
import java.nio.channels.SocketChannel
import java.nio.charset.StandardCharsets
import java.nio.file.Files
import java.nio.file.Path
import java.nio.file.Paths
import java.time.format.DateTimeFormatter
import java.util.concurrent.atomic.AtomicBoolean
import kotlin.concurrent.thread

/**
 * Minimal HTTP server used by the sample to expose a status page and JSON endpoint.
 *
 * In snap mode it can listen on a Unix domain socket; otherwise it uses localhost TCP.
 */
class ProviderWebServer(
  private val port: Int,
  private val unixSocketPath: String?
) : AutoCloseable {
  private val running = AtomicBoolean(false)
  private var unixServerThread: Thread? = null
  private var unixServerChannel: ServerSocketChannel? = null
  private var tcpServer: com.sun.net.httpserver.HttpServer? = null
  private var effectiveUnixPath: String? = null

  /** Maps incoming request paths to static sample responses. */
  private fun handle(path: String): Pair<Int, Pair<String, String>> {
    // Normalize path: handle both with and without the reverse proxy prefix
    val normalizedPath = when {
      path.startsWith("/sdk-kotlin-provider-simple/") -> path.substring("/sdk-kotlin-provider-simple".length)
      path.startsWith("/sdk-kotlin-provider-simple") -> "/"
      else -> path
    }

    return when (normalizedPath) {
      "/", "/index.html" -> 200 to ("text/html; charset=utf-8" to htmlPage())
      "/api/provider" -> 200 to ("application/json; charset=utf-8" to jsonPayload())
      else -> 404 to ("application/json; charset=utf-8" to "{\"error\":\"not found\"}")
    }
    }

    /** Starts exactly one server instance (Unix socket preferred in snap context). */
    fun start() {
    if (!running.compareAndSet(false, true)) {
      return
    }

    if (!unixSocketPath.isNullOrBlank()) {
      startUnixSocketServer(unixSocketPath)
      return
    }

    val server = com.sun.net.httpserver.HttpServer.create(InetSocketAddress("127.0.0.1", port), 0)
    server.createContext("/") { exchange ->
      val (statusCode, body) = handle(exchange.requestURI.path)
      val (contentType, content) = body
      writeTcpResponse(exchange, statusCode, contentType, content)
    }
    server.start()
    tcpServer = server
    println("Web server started on 127.0.0.1:$port")
    }

    /** Stops servers, closes resources and removes the Unix socket file if present. */
    override fun close() {
    running.set(false)
    tcpServer?.stop(0)
    tcpServer = null

    unixServerChannel?.close()
    unixServerChannel = null

    unixServerThread?.join(1000)
    unixServerThread = null

    val pathToDelete = effectiveUnixPath
    if (!pathToDelete.isNullOrBlank()) {
      Files.deleteIfExists(Paths.get(pathToDelete))
    }
  }

  private fun startUnixSocketServer(path: String) {
    val socketPath = Paths.get(path)
    Files.createDirectories(socketPath.parent)
    Files.deleteIfExists(socketPath)

    val channel = ServerSocketChannel.open(StandardProtocolFamily.UNIX)
    channel.bind(UnixDomainSocketAddress.of(path))

    unixServerChannel = channel
    effectiveUnixPath = path

    // Handle one client per accepted connection; this is sufficient for a sample page.
    unixServerThread = thread(name = "provider-webserver-unix", isDaemon = true) {
      while (running.get()) {
        val client = try {
          channel.accept()
        } catch (_: Throwable) {
          null
        } ?: break

        client.use { socket -> handleUnixClient(socket) }
      }
    }

    println("Web server started on unix://$path")
  }

  private fun handleUnixClient(socket: SocketChannel) {
    val rawRequest = readRequest(socket)
    val firstLine = rawRequest.lineSequence().firstOrNull().orEmpty()
    val requestPath = firstLine.split(' ').getOrNull(1) ?: "/"

    val (statusCode, body) = handle(requestPath)
    val (contentType, content) = body
    writeUnixResponse(socket, statusCode, contentType, content)
  }

  /** Reads only request headers, which is enough for GET endpoints in this sample. */
  private fun readRequest(socket: SocketChannel): String {
    val buffer = ByteBuffer.allocate(1024)
    val bytes = ByteArrayOutputStream()

    while (true) {
      val read = socket.read(buffer)
      if (read <= 0) {
        break
      }
      buffer.flip()
      val chunk = ByteArray(buffer.remaining())
      buffer.get(chunk)
      bytes.write(chunk)
      buffer.clear()

      val content = bytes.toString(StandardCharsets.UTF_8)
      if (content.contains("\r\n\r\n")) {
        break
      }
    }

    return bytes.toString(StandardCharsets.UTF_8)
  }

  private fun writeUnixResponse(socket: SocketChannel, statusCode: Int, contentType: String, body: String) {
    val responseBytes = body.toByteArray(StandardCharsets.UTF_8)
    val statusText = if (statusCode == 200) "OK" else "Not Found"
    val response = buildString {
      append("HTTP/1.1 $statusCode $statusText\r\n")
      append("Content-Type: $contentType\r\n")
      append("Content-Length: ${responseBytes.size}\r\n")
      append("Connection: close\r\n")
      append("\r\n")
    }.toByteArray(StandardCharsets.UTF_8)

    socket.write(ByteBuffer.wrap(response))
    socket.write(ByteBuffer.wrap(responseBytes))
    }

    /** Builds the HTML sample page shown in a browser. */
    private fun htmlPage(): String {
        val timestamp = DateTimeFormatter.ISO_INSTANT.format(ProviderValues.currentTimestampValue())
        val stringValue = ProviderValues.peekStringValue()
        val integerValue = ProviderValues.currentIntegerValue()
        return """
            <html>
              <head>
                <meta charset="utf-8" />
                <title>ctrlX Kotlin Provider</title>
                <style>
                  body { font-family: sans-serif; margin: 2rem; line-height: 1.5; }
                  code { background: #f4f4f4; padding: 0.15rem 0.35rem; border-radius: 0.25rem; }
                  .card { padding: 1rem 1.25rem; border: 1px solid #ddd; border-radius: 0.75rem; max-width: 50rem; }
                </style>
              </head>
              <body>
                <div class="card">
                  <h1>ctrlX Kotlin Provider</h1>
                  <p>This page reads the provider value via the ctrlX Data Layer REST API.</p>
                  <p>String (REST): <code id="stringValue">loading...</code></p>
                  <p>Integer (local API): <code>$integerValue</code></p>
                  <p>Timestamp (local API): <code>$timestamp</code></p>
                  <p>Fallback String (local API): <code>$stringValue</code></p>
                  <p>Try JSON endpoint: <code>/api/provider</code></p>
                </div>
                <script>
                  (async () => {
                    const target = document.getElementById('stringValue');
                    try {
                      const response = await fetch('/automation/api/v2/nodes/sdk/kotlin/provider/simple/string');
                      const json = await response.json();
                      target.textContent = JSON.stringify(json);
                    } catch (e) {
                      target.textContent = 'REST call failed';
                    }
                  })();
                </script>
              </body>
            </html>
        """.trimIndent()
    }

    /** Returns a compact JSON view of current provider values. */
    private fun jsonPayload(): String {
        val timestamp = DateTimeFormatter.ISO_INSTANT.format(ProviderValues.currentTimestampValue())
        return """
            {
              "string": "${ProviderValues.peekStringValue()}",
              "int32": ${ProviderValues.currentIntegerValue()},
              "timestamp": "$timestamp",
              "flatbuffers": { "x": 1, "y": 2, "z": 3 }
            }
        """.trimIndent()
    }

    /** Writes a complete HTTP response for TCP requests. */
    private fun writeTcpResponse(exchange: com.sun.net.httpserver.HttpExchange, statusCode: Int, contentType: String, body: String) {
        val responseBytes = body.toByteArray(StandardCharsets.UTF_8)
        exchange.responseHeaders.add("Content-Type", contentType)
        exchange.sendResponseHeaders(statusCode, responseBytes.size.toLong())
        exchange.responseBody.use { output -> output.write(responseBytes) }
    }
}