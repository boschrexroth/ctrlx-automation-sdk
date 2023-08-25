/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
using Hello.Web.Asp.services;

namespace Hello.Web.Asp
{
    using Microsoft.AspNetCore.Builder;
    using Microsoft.AspNetCore.Hosting;
    using Microsoft.AspNetCore.Http;
    using Microsoft.Extensions.DependencyInjection;
    using Microsoft.Extensions.Hosting;

    /// <summary>
    /// Defines the <see cref="Startup" />.
    /// </summary>
    public class Startup
    {
        // This method gets called by the runtime. Use this method to add services to the container.
        // For more information on how to configure your application, visit https://go.microsoft.com/fwlink/?LinkID=398940
        /// <summary>
        /// The ConfigureServices.
        /// </summary>
        /// <param name="services">The services<see cref="IServiceCollection"/>.</param>
        public void ConfigureServices(IServiceCollection services)
        {
            services.AddSingleton(new GreetService("Hello Web powered by ASP.NET!"));
            services.AddSingleton(new DataLayerService());
        }

        // This method gets called by the runtime. Use this method to configure the HTTP request pipeline.
        /// <summary>
        /// The Configure.
        /// </summary>
        /// <param name="app">The app<see cref="IApplicationBuilder"/>.</param>
        /// <param name="env">The env<see cref="IWebHostEnvironment"/>.</param>
        public void Configure(IApplicationBuilder app, IWebHostEnvironment env)
        {
            if (env.IsDevelopment())
            {
                app.UseDeveloperExceptionPage();
            }

            app.UseRouting();
            app.Run(async context =>
            {
                var greeter = context.RequestServices.GetService<GreetService>();
                if (greeter == null)
                {
                    await context.Response.WriteAsync("Greeter is null\n");
                }
                else
                {
                    await context.Response.WriteAsync($"{greeter.Say()}\n");
                }

                var dataLayerService = context.RequestServices.GetService<DataLayerService>();
                if (dataLayerService == null)
                {
                    await context.Response.WriteAsync("DataLayerService is null\n");
                }
                else
                {
                    await context.Response.WriteAsync($"{dataLayerService.ReadNodeValue("framework/metrics/system/cpu-utilisation-percent")}\n");
                    await context.Response.WriteAsync($"{dataLayerService.ReadNodeValue("framework/metrics/system/memavailable-mb")}\n");
                    await context.Response.WriteAsync($"{dataLayerService.ReadNodeValue("framework/metrics/system/membuffers-mb")}\n");
                    await context.Response.WriteAsync($"{dataLayerService.ReadNodeValue("framework/metrics/system/memcache-mb")}\n");
                    await context.Response.WriteAsync($"{dataLayerService.ReadNodeValue("framework/metrics/system/memfree-mb")}\n");
                    await context.Response.WriteAsync($"{dataLayerService.ReadNodeValue("framework/metrics/system/memtotal-mb")}\n");
                    await context.Response.WriteAsync($"{dataLayerService.ReadNodeValue("framework/metrics/system/memused-mb")}\n");
                    await context.Response.WriteAsync($"{dataLayerService.ReadNodeValue("framework/metrics/system/memused-percent")}\n");
                }
            });
        }
    }
}
