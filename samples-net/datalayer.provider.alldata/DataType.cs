/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
namespace Samples.Datalayer.Provider.Alldata
{
    /// <summary>
    /// Defines the <see cref="DataType" />.
    /// </summary>
    internal class DataType
    {
        /// <summary>
        /// Gets the Name.
        /// </summary>
        public string Name { get; }

        /// <summary>
        /// Gets the Address.
        /// </summary>
        public string Address { get; }

        /// <summary>
        /// Initializes a new instance of the <see cref="DataType"/> class.
        /// </summary>
        /// <param name="name">The name<see cref="string"/>.</param>
        /// <param name="address">The address<see cref="string"/>.</param>
        public DataType(string name, string address)
        {
            Name = name;
            Address = address;
        }
    }
}
