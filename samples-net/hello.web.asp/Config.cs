
namespace Hello.Web.Asp
{
    /// <summary>
    /// Defines the <see cref="Config" />.
    /// </summary>
    public class Config
    {

        // This is the connection string for TCP in the format: tcp://USER:PASSWORD@IP_ADDRESS:DATALAYER_PORT?sslport=SSL_PORT
        // Please check and change according your environment:
        // - USER:        Enter your user name here - default is boschrexroth
        // - PASSWORD:    Enter your password here - default is boschrexroth
        // - IP_ADDRESS:
        //   127.0.0.1    If you develop on your (Windows) host and you want to connect to a ctrlX CORE virtual with port forwarding
        //   10.0.2.2     If you develop on a VM (QEMU, Virtual Box) and you want to connect to a ctrlX virtual with port forwarding
        //   192.168.1.1  If you are using a ctrlX CORE or ctrlX CORE virtual with TAP adpater
        // - DATALAYER_PORT:
        //   2069         The ctrlX Data Layer client port
        //   2070         The ctrlX Data Layer provider port
        // - SSL_PORT:
        //   443          Default SSL Port if you are using a ctrlX CORE or ctrlX CORE virtual with TAP adpater
        //   8443         Default forwarded SSL Port if you are using a ctrlX CORE virtual

        // Please change the following constants according to your environment
        public const string USER = "boschrexroth";
        public const string PASSWORD = "boschrexroth";
        public const string IP_ADDRESS = "10.0.2.2";
        public const int SSL_PORT = 8443;
    }
}