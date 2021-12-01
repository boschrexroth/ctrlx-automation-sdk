namespace Hello.Web.Asp.services
{
    /// <summary>
    /// Defines the <see cref="GreetService" />.
    /// </summary>
    public class GreetService
    {
        /// <summary>
        /// Gets the Greeting.
        /// </summary>
        public string Greeting { get; }

        /// <summary>
        /// Initializes a new instance of the <see cref="GreetService"/> class.
        /// </summary>
        /// <param name="greeting">The greeting<see cref="string"/>.</param>
        public GreetService(string greeting)
        {
            Greeting = greeting;
        }

        /// <summary>
        /// The Say.
        /// </summary>
        /// <returns>The <see cref="string"/>.</returns>
        public string Say() => Greeting;
    }
}
