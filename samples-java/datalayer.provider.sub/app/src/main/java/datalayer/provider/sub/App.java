/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

package datalayer.provider.sub;

import java.lang.System.Logger.Level;
import java.util.Enumeration;
import java.util.Map;
import java.util.Properties;
import com.boschrexroth.api.Client;
import com.boschrexroth.api.DlSystem;
import com.boschrexroth.api.Provider;
import com.boschrexroth.api.RemoteBuilder;

public class App {

    private static final java.lang.System.Logger LOG = java.lang.System.getLogger(App.class.getName());

    public static void main(String[] args) {

        for (String arg : args) {
            LOG.log(Level.INFO, "main({0})", arg);
        }

        // getEnv();

        try (DlSystem s = DlSystem.create("")) {
            s.start(false);

            String remote = RemoteBuilder.create().IpVirtual().PortVirtual().build();
            LOG.log(Level.INFO, "start: " + remote);

            Runtime.getRuntime().addShutdownHook(new Thread(() -> {
                LOG.log(Level.INFO, "\nShutdown hook activated! Start cleanup...");
                RunUtils.running.set(false);
            }));

            if (args == null || args[0].equals("provider")) {
                runProvider(s, remote);
                return;
            }

            runClient(s, remote);

        } catch (Exception e) {
            LOG.log(Level.ERROR, e.getMessage(), e);
        }
    }

    private static void runClient(DlSystem s, String remote) {
        try (Client client = s.Factory().createClient(remote)) {
            RunClient.run(client);
        } catch (Exception e) {
            LOG.log(Level.INFO, e.getMessage(), e);
        }
    }

    private static void runProvider(DlSystem s, String remote) throws Exception {
        try (Provider provider = s.Factory().createProvider(remote)) {
            RunProvider.run(provider);
        }
    }

    private static void getEnv() {
        LOG.log(Level.INFO, "All Enviroments ");
        Map<String, String> env = System.getenv();
        for (String envName : env.keySet()) {
            LOG.log(Level.INFO, "{0}={1}", envName, env.get(envName));
        }

        LOG.log(Level.INFO, "\nAll Java Properties ");
        Properties props = System.getProperties();
        Enumeration<?> propNames = props.propertyNames();
        for (; propNames.hasMoreElements();) {
            String key = propNames.nextElement().toString();
            LOG.log(Level.INFO, "{0}: {1}", key, props.getProperty(key));
        }
    }
}
