The assignment brief has made it very unclear what the requirements are.

```txt
Pre-processor directives should be embedded within your project to enable / disable compilation of client and server code. 
This should allow you to develop a single project that contains both the Client and Server code.

Your submission should include two projects, one configured to be a server and another client.
```

I am assuming that this means that the submissions should include the server and client projects, but it can include more:

Therefore I have submitted a third project called `shared` that contains
- `server` code
- `client` code
- `shared` code - in the form of the `Comms` class

I did this because it seemed illogical to copy and paste the code in both the `server` and `client` projects

`server` and `client` contain minimal code that references the `shared` project

The `shared` project contains the required Pre-processor directives to enable / disable compilation of client and server code
