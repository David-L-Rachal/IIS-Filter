# IIS Filter

A sanitized historical snapshot of a production C++ security component I worked on for **Westwood One**.

## Background

This project was an in-house website security layer developed for Westwood One and used in support of **ISIS**, an internal application used by on-air radio personnel to organize traffic-incident information for broadcast.

The filter ran as a native **Microsoft IIS ISAPI filter**, intercepting incoming HTTP requests before they reached the underlying application.

This repository is preserved as an example of production C++ and web infrastructure work from earlier in my software-development career.

## What It Did

The filter operated inside IIS and provided an authentication and authorization layer in front of the application.

At a high level, it:

- Intercepted HTTP requests using the IIS ISAPI filter API.
- Examined request headers and requested URLs.
- Handled HTTP Basic authentication.
- Queried a SQL-backed security data source.
- Maintained an in-memory authorization cache to reduce repeated database lookups.
- Matched authenticated users against protected application paths.
- Allowed or rejected requests before they reached the underlying website.
- Returned the appropriate authentication challenge when access was denied.

The main IIS integration can be found in `IIS_Filter.cpp`, including the `HttpFilterProc` and `GetFilterVersion` entry points.

## StringBuilder

The repository also contains `stringbuilder.h`, a sample of a reusable C++ string utility I helped develop for the larger ISIS codebase.

This predates many of the conveniences I would use in modern C++, and it includes functionality for dynamic string construction, concatenation, conversion, comparison, parsing, and other operations that were useful throughout the application.

It is included here because it represents another part of the engineering work surrounding ISIS rather than being specific only to the IIS security filter.

## Technology

The original project used technologies including:

- C++
- Microsoft IIS / ISAPI
- Win32
- HTTP authentication
- SQL / OLE DB
- ADO
- Visual C++
- Custom C++ utility classes

The original Visual C++ project files are retained to preserve the historical structure of the project.

## Historical Code

This is **legacy production code**, not a modern reference implementation.

I have intentionally left much of the original structure intact because the purpose of this repository is to show an authentic example of the systems I worked on at that point in my career.

If I were designing the same system today, I would make substantially different choices around authentication, credential handling, configuration, memory management, database access, observability, testing, and separation of responsibilities.

That evolution is part of why I wanted to preserve this project.

## Security & Sanitization

This repository has been sanitized specifically for public release.

Before publication, I removed or replaced:

- Production credentials
- Database usernames and passwords
- Internal server and host names
- Internal domain information
- FTP/deployment credentials
- Deployment scripts containing environment-specific information
- Compiled binaries and debugging symbols
- Build artifacts
- Historical archives
- Legacy source-control metadata

Remaining configuration examples use placeholders or intentionally generic sample values.

The code in this repository should **not** be deployed as a security system in its current form.

## About This Repository

This isn't a newly written portfolio demonstration.

It is preserved production-era code from earlier in my career, cleaned for public viewing. I'm publishing it alongside newer projects to show some of the progression in the kinds of systems I've designed and built over the years.
