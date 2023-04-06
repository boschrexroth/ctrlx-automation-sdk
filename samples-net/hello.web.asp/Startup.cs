/*
MIT License

Copyright (c) 2021-2023 Bosch Rexroth AG

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
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
