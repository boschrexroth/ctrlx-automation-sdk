/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

package datalayer.provider.simple

import com.boschrexroth.api.DlSystem
import com.boschrexroth.api.Provider
import com.boschrexroth.api.ProviderNode
import com.boschrexroth.api.RemoteBuilder
import java.nio.file.Paths
import java.util.concurrent.TimeUnit
import java.util.concurrent.atomic.AtomicBoolean

private val running = AtomicBoolean(true)

private const val ADDRESS_ROOT = "sdk/kotlin/provider/simple/"
private const val TYPE_ADDRESS_FBS = "types/${ADDRESS_ROOT}inertial-value"
private const val HTTP_SERVER_PORT = 8080
private const val SNAP_NAME = "sdk-kotlin-provider-simple"
private const val SNAP_UNIX_SOCKET = "web.sock"

/**
 * Keeps the provider alive until a shutdown signal is received.
 *
 * The loop also exits early if the provider connection drops.
 */
private fun waitHandler(provider: Provider) {
    try {
        while (running.get()) {
            if (!provider.isConnected) {
                println("connecting failed")
                return
            }
            TimeUnit.SECONDS.sleep(2)
        }
    } catch (e: InterruptedException) {
        Thread.currentThread().interrupt()
        println("Main thread interrupted. End of loop")
    }
}

/**
 * Creates all provider nodes, registers type information and starts the provider runtime.
 */
private fun run(provider: Provider) {
    println("Run begin")

    if (!running.get()) return

    val callbackString = ProviderNodeCallbackString()
    val callbackInteger = ProviderNodeCallbackInteger()
    val callbackTimestamp = ProviderNodeCallbackTimestamp()
    val callbackFbs = ProviderNodeCallbackFbs(TYPE_ADDRESS_FBS)

    try {
        val snapData = System.getenv("SNAP_DATA")
        val unixSocketPath = if (!snapData.isNullOrBlank()) {
            Paths.get(snapData, "package-run", SNAP_NAME, SNAP_UNIX_SOCKET).toString()
        } else {
            null
        }

        ProviderWebServer(HTTP_SERVER_PORT, unixSocketPath).use { webServer ->
            webServer.start()

        ProviderNode.create(callbackString).use { node ->
        ProviderNode.create(callbackInteger).use { nodeInteger ->
        ProviderNode.create(callbackTimestamp).use { nodeTimestamp ->
        ProviderNode.create(callbackFbs).use { nodeFbs ->

            var res = provider.registerNode("${ADDRESS_ROOT}string", node)
            if (res != com.boschrexroth.api.Result.OK) {
                println("Error: registerNode: ${ADDRESS_ROOT}string")
            }

            res = provider.registerNode("${ADDRESS_ROOT}int32", nodeInteger)
            if (res != com.boschrexroth.api.Result.OK) {
                println("Error: registerNode: ${ADDRESS_ROOT}int32")
            }

            res = provider.registerNode("${ADDRESS_ROOT}timestamp", nodeTimestamp)
            if (res != com.boschrexroth.api.Result.OK) {
                println("Error: registerNode: ${ADDRESS_ROOT}timestamp")
            }

            var bfbsPath = Paths.get(System.getProperty("user.dir"), "bfbs", "sampleSchema.bfbs").toString()
            val snap = System.getenv("SNAP")
            if (!snap.isNullOrEmpty()) {
                bfbsPath = Paths.get(snap, "bfbs", "sampleSchema.bfbs").toString()
            }

            res = provider.registerType(TYPE_ADDRESS_FBS, bfbsPath)
            if (res != com.boschrexroth.api.Result.OK) {
                println("Error: registerType: $TYPE_ADDRESS_FBS path: $bfbsPath")
            }

            res = provider.registerNode("${ADDRESS_ROOT}inertial-value", nodeFbs)
            if (res != com.boschrexroth.api.Result.OK) {
                println("Error: registerNode: ${ADDRESS_ROOT}inertial-value")
            }

            provider.start()
            waitHandler(provider)
        }}}}
        }
    } catch (e: Throwable) {
        e.printStackTrace()
    }

    println("Run end")
}

/**
 * Starts the ctrlX Data Layer system and runs the simple provider example.
 */
fun main(args: Array<String>) {
    printEnv()

    DlSystem.create("").use { system ->
        system.start(false)

        val remote = RemoteBuilder.create().IpVirtual().PortVirtual().build()
        println("start: $remote")

        Runtime.getRuntime().addShutdownHook(Thread {
            println("\nShutdown hook activated! Start cleanup...")
            running.set(false)
        })

        system.Factory().createProvider(remote).use { provider ->
            run(provider)
        }
    }
}

/**
 * Prints environment variables and JVM properties to ease troubleshooting in local and snap runs.
 */
private fun printEnv() {
    println("All Environments")
    System.getenv().forEach { (name, value) -> println("$name=$value") }

    println("\nAll Java Properties")
    System.getProperties().forEach { key, value -> println("$key : $value") }
}
