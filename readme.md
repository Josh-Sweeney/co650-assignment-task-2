## CO650 Assignment Task 2

This repository contains the source code for my CO650 Assignment task 2 submission.

## Project Structure

The assignment brief states

```txt
Pre-processor directives should be embedded within your project to enable / disable compilation of client and server code. 
This should allow you to develop a single project that contains both the Client and Server code.

Your submission should include two projects, one configured to be a server and another client.
```

Therefore I have created three projects
- A `server` application
- A `client` application
- A `shared` static library

The `server` and `client` projects reference the `server` project.

The `shared` library contains the client, server and common code. This library makes use of the `COMPILE_CLIENT` and `COMPILE_SERVER` pre-processor definitions to compile either the client or server code.

In order to make use of the pre-processor directives, I created a `Server` and `Client` configuration
- The `Server` configuration defines the `COMPILE_SERVER` directive, but does not define the `COMPILE_CLIENT` directive which means only the server code is compiled
- The `Client` configuration defined the `COMPILE_CLIENT` directive, but does not define the `COMPILE_SERVER` directive which means only the client code is compiled

## How To Run

### Visual Studio

This is the recommended way to run the applications.

Open `co650-assignment-task-2.sln`

#### Server

If you intend to run the `server`, select the `Server` configuration and right click on the `server` project in the Solution Explorer and select `Debug > Start Without Debugging`
- This approach ensures that only the server code is compiled

#### Client

If you intend to run the `client`, select the `Client` configuration and right click on the `client` project in the Solution Explorer and select `Debug > Start Without Debugging`
- This approach ensures that only the client code is compiled

#### Debugging

To debug either project, select the `Debug` configuration
- Note that this will compile both the server and client code
