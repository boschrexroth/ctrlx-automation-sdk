# Crate Documentation

**Version:** 1.1.0

**Format Version:** 57

# Module `ctrlx_datalayer`

## Modules

## Module `result`

Result codes for the ctrlX Data Layer.

These constants correspond to the `DLR_RESULT_DL_*` values defined in the
C API (`comm/datalayer/c/comm_datalayer.h`).

```rust
pub mod result { /* ... */ }
```

### Types

#### Enum `Result`

Represents the Datalayer Result Values

```rust
pub enum Result {
    Ok,
    OkNoContent,
    OkWithErrors,
    Failed,
    InvalidAddress,
    Unsupported,
    OutOfMemory,
    LimitMin,
    LimitMax,
    TypeMismatch,
    SizeMismatch,
    InvalidFloatingpoint,
    InvalidHandle,
    InvalidOperationMode,
    InvalidConfiguration,
    InvalidValue,
    SubmoduleFailure,
    Timeout,
    AlreadyExists,
    CreationFailed,
    VersionMismatch,
    Deprecated,
    PermissionDenied,
    NotInitialized,
    MissingArgument,
    TooManyArguments,
    ResourceUnavailable,
    CommunicationError,
    TooManyOperations,
    WouldBlock,
    CommProtocolError,
    CommInvalidHeader,
    ClientNotConnected,
    ProviderResetTimeout,
    ProviderUpdateTimeout,
    ProviderSubHandling,
    RtNotopen,
    RtInvalidobject,
    RtWrongrevison,
    RtNovaliddata,
    RtMemorylocked,
    RtInvalidmemorymap,
    RtInvalidRetain,
    RtInternalError,
    RtMallocFailed,
    RtWouldBlock,
    SecNoToken,
    SecInvalidSession,
    SecInvalidTokenContent,
    SecUnauthorized,
    SecPaymentRequired,
    Unknown(std::os::raw::c_uint),
}
```

##### Variants

###### `Ok`

Function call succeeded.

###### `OkNoContent`

Function call succeeded with no content.

###### `OkWithErrors`

Function call succeeded with errors.

###### `Failed`

Function call failed.

###### `InvalidAddress`

Invalid address.

###### `Unsupported`

Unsupported operation.

###### `OutOfMemory`

Out of memory.

###### `LimitMin`

Limit minimum reached.

###### `LimitMax`

Limit maximum reached.

###### `TypeMismatch`

Type mismatch.

###### `SizeMismatch`

Size mismatch.

###### `InvalidFloatingpoint`

Invalid floating point.

###### `InvalidHandle`

Invalid handle.

###### `InvalidOperationMode`

Invalid operation mode.

###### `InvalidConfiguration`

Invalid configuration.

###### `InvalidValue`

Invalid value.

###### `SubmoduleFailure`

Submodule failure.

###### `Timeout`

Timeout occurred.

###### `AlreadyExists`

Resource already exists.

###### `CreationFailed`

Creation failed.

###### `VersionMismatch`

Version mismatch.

###### `Deprecated`

Deprecated function or feature.

###### `PermissionDenied`

Permission denied.

###### `NotInitialized`

Not initialized.

###### `MissingArgument`

Missing argument.

###### `TooManyArguments`

Too many arguments.

###### `ResourceUnavailable`

Resource unavailable.

###### `CommunicationError`

Communication error.

###### `TooManyOperations`

Too many operations.

###### `WouldBlock`

Operation would block.

###### `CommProtocolError`

Communication protocol error.

###### `CommInvalidHeader`

Invalid communication header.

###### `ClientNotConnected`

Client not connected.

###### `ProviderResetTimeout`

Provider reset timeout.

###### `ProviderUpdateTimeout`

Provider update timeout.

###### `ProviderSubHandling`

Provider subscription handling error.

###### `RtNotopen`

Runtime not open.

###### `RtInvalidobject`

Invalid runtime object.

###### `RtWrongrevison`

Wrong runtime revision.

###### `RtNovaliddata`

No valid runtime data.

###### `RtMemorylocked`

Runtime memory locked.

###### `RtInvalidmemorymap`

Invalid runtime memory map.

###### `RtInvalidRetain`

Invalid runtime retain operation.

###### `RtInternalError`

Internal runtime error.

###### `RtMallocFailed`

Runtime memory allocation failed.

###### `RtWouldBlock`

Runtime operation would block.

###### `SecNoToken`

No security token provided.

###### `SecInvalidSession`

Token not valid (session not found).

###### `SecInvalidTokenContent`

Token has wrong content.

###### `SecUnauthorized`

Unauthorized.

###### `SecPaymentRequired`

Payment required.

###### `Unknown`

Used if the C library returns an unknown error code.

Fields:

| Index | Type | Documentation |
|-------|------|---------------|
| 0 | `std::os::raw::c_uint` |  |

##### Implementations

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Clone**
  - ```rust
    fn clone(self: &Self) -> Result { /* ... */ }
    ```

- **CloneToUninit**
  - ```rust
    unsafe fn clone_to_uninit(self: &Self, dest: *mut u8) { /* ... */ }
    ```

- **Copy**
- **Debug**
  - ```rust
    fn fmt(self: &Self, f: &mut $crate::fmt::Formatter<''_>) -> $crate::fmt::Result { /* ... */ }
    ```

- **Display**
  - ```rust
    fn fmt(self: &Self, f: &mut fmt::Formatter<''_>) -> fmt::Result { /* ... */ }
    ```

- **Eq**
- **Error**
- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

  - ```rust
    fn from(err_code: std::os::raw::c_uint) -> Self { /* ... */ }
    ```

  - ```rust
    fn from(result: Result) -> Self { /* ... */ }
    ```

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **PartialEq**
  - ```rust
    fn eq(self: &Self, other: &Result) -> bool { /* ... */ }
    ```

- **RefUnwindSafe**
- **Send**
- **StructuralPartialEq**
- **Sync**
- **ToOwned**
  - ```rust
    fn to_owned(self: &Self) -> T { /* ... */ }
    ```

  - ```rust
    fn clone_into(self: &Self, target: &mut T) { /* ... */ }
    ```

- **ToString**
  - ```rust
    fn to_string(self: &Self) -> String { /* ... */ }
    ```

- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
## Module `system`

```rust
pub mod system { /* ... */ }
```

### Types

#### Struct `System`

Represents a ctrlX Data Layer system instance.

The `System` is the root object of the Data Layer. It manages the lifecycle
of the underlying native system handle and provides access to the factory
and converter subsystems.
```ignore
let system = System::new("");
system.start(false);
{
  ...
}
info!("Shutdown!");
```

```rust
pub struct System {
    // Some fields omitted
}
```

##### Fields

| Name | Type | Documentation |
|------|------|---------------|
| *private fields* | ... | *Some fields have been omitted* |

##### Implementations

###### Methods

- ```rust
  pub fn new(path: &str) -> Self { /* ... */ }
  ```
  Creates a new `System` instance.

- ```rust
  pub fn start(self: &Self, start_broker: bool) { /* ... */ }
  ```
  Starts the Data Layer system.

- ```rust
  pub fn stop(self: &Self, force_provider_stop: bool) -> bool { /* ... */ }
  ```
  Stops the Data Layer system.

- ```rust
  pub fn factory(self: &Self) -> crate::factory::Factory { /* ... */ }
  ```
  Returns the [`Factory`](crate::factory::Factory) associated with this system.

- ```rust
  pub fn converter(self: &Self) -> crate::converter::Converter { /* ... */ }
  ```
  Returns the [`Converter`](crate::converter::Converter) associated with this system.

- ```rust
  pub fn update_bfbs_path(self: &Self, path: &str) { /* ... */ }
  ```
  Sets the path to the BFBS (Binary FlatBuffers Schema) file.

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Drop**
  - ```rust
    fn drop(self: &mut Self) { /* ... */ }
    ```
    Deletes the native system handle when the `System` is dropped.

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
## Module `factory`

```rust
pub mod factory { /* ... */ }
```

### Types

#### Struct `Factory`

Provides access to the Data Layer factory.

The `Factory` is obtained from a [`System`](crate::system::System) and is used
to create clients and providers that interact with the Data Layer.

```rust
pub struct Factory {
    // Some fields omitted
}
```

##### Fields

| Name | Type | Documentation |
|------|------|---------------|
| *private fields* | ... | *Some fields have been omitted* |

##### Implementations

###### Methods

- ```rust
  pub fn create_client(self: &Self, connection: Connection) -> crate::client::Client { /* ... */ }
  ```
  Creates a new [`Client`](crate::client::Client) connected to the Data Layer.

- ```rust
  pub fn create_provider(self: &Self, connection: Connection) -> crate::provider::Provider { /* ... */ }
  ```
  Creates a new [`Provider`](crate::provider::Provider) connected to the Data Layer.

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Drop**
  - ```rust
    fn drop(self: &mut Self) { /* ... */ }
    ```
    Resets the native factory handle when the `Factory` is dropped.

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
#### Enum `Connection`

Describes how a client or provider connects to the Data Layer broker.

```rust
pub enum Connection {
    Ipc,
    IpVirtual,
    Ip(String, String, String),
    RemoteBuilder(crate::connection_builder::ConnectionString),
}
```

##### Variants

###### `Ipc`

Connects via IPC (inter-process communication) on the same device.

###### `IpVirtual`

Connects via TCP to a virtual machine at the default address (`10.0.2.2?sslport=8443`).

###### `Ip`

Connects via TCP to a custom host with given user credentials.

Fields: `(host, user, password)`.

Fields:

| Index | Type | Documentation |
|-------|------|---------------|
| 0 | `String` |  |
| 1 | `String` |  |
| 2 | `String` |  |

###### `RemoteBuilder`

Uses a fully custom connection string (e.g. built by the caller).

Fields:

| Index | Type | Documentation |
|-------|------|---------------|
| 0 | `crate::connection_builder::ConnectionString` |  |

##### Implementations

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
## Module `client`

```rust
pub mod client { /* ... */ }
```

### Modules

## Module `async`

```rust
pub mod async { /* ... */ }
```

### Types

#### Type Alias `ResponseCallbackBox`

Type alias for a boxed [`ResponseCallback`].

```rust
pub type ResponseCallbackBox = Box<dyn ResponseCallback>;
```

#### Struct `AsyncClient`

Provides asynchronous access to the Data Layer.

An `AsyncClient` is obtained via [`Client::async`](crate::client::Client::async).
It holds the same `DLR_CLIENT` handle as the owning [`Client`](crate::client::Client)
and is dropped automatically when the `Client` is dropped.

```rust
pub struct AsyncClient {
    // Some fields omitted
}
```

##### Fields

| Name | Type | Documentation |
|------|------|---------------|
| *private fields* | ... | *Some fields have been omitted* |

##### Implementations

###### Methods

- ```rust
  pub fn ping(self: &Self, callback: ResponseCallbackBox) -> crate::Result { /* ... */ }
  ```
  Sends an asynchronous ping to the Data Layer to check connectivity.

- ```rust
  pub fn read(self: &Self, address: &str, arg: Option<crate::Value>, callback: ResponseCallbackBox) -> crate::Result { /* ... */ }
  ```
  Reads the value at `address` asynchronously.

- ```rust
  pub fn write(self: &Self, address: &str, arg: Option<crate::Value>, callback: ResponseCallbackBox) -> crate::Result { /* ... */ }
  ```
  Writes `arg` to `address` asynchronously.

- ```rust
  pub fn browse(self: &Self, address: &str, callback: ResponseCallbackBox) -> crate::Result { /* ... */ }
  ```
  Browses the child nodes of `address` asynchronously.

- ```rust
  pub fn metadata(self: &Self, address: &str, callback: ResponseCallbackBox) -> crate::Result { /* ... */ }
  ```
  Retrieves the metadata of `address` asynchronously.

- ```rust
  pub fn remove(self: &Self, address: &str, callback: ResponseCallbackBox) -> crate::Result { /* ... */ }
  ```
  Removes the node at `address` asynchronously.

- ```rust
  pub fn create(self: &Self, address: &str, arg: Option<crate::Value>, callback: ResponseCallbackBox) -> crate::Result { /* ... */ }
  ```
  Creates a new node at `address` asynchronously.

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
### Traits

#### Trait `ResponseCallback`

Callback trait for asynchronous Data Layer responses.

Implement this trait to receive the result and data of an async operation.
The `Send` bound is required because callbacks are invoked from a C thread.
```ignore
struct MyCallback;
impl ResponseCallback for MyCallback {
    fn on_response(&mut self, res: crate::Result, data: crate::Value) {
        // Handle the response here
    }
}
```

```rust
pub trait ResponseCallback: Send {
    /* Associated items */
}
```

##### Required Items

###### Required Methods

- `on_response`

## Module `bulk`

```rust
pub mod bulk { /* ... */ }
```

### Modules

## Module `creator_bulk`

```rust
pub mod creator_bulk { /* ... */ }
```

## Module `sync_bulk`

```rust
pub mod sync_bulk { /* ... */ }
```

### Types

#### Struct `SyncBulk`

Provides synchronous bulk read operations on the Data Layer.

`SyncBulk` is created via [`Client::create_sync_bulk`](crate::client::Client::create_sync_bulk)
and allows reading multiple nodes in a single round-trip.

```rust
pub struct SyncBulk {
    // Some fields omitted
}
```

##### Fields

| Name | Type | Documentation |
|------|------|---------------|
| *private fields* | ... | *Some fields have been omitted* |

##### Implementations

###### Methods

- ```rust
  pub fn read(self: &Self, args: &[BulkReadArg]) -> Result<Vec<BulkResponse>, crate::Result> { /* ... */ }
  ```
  Reads multiple Data Layer nodes in a single synchronous bulk request.

- ```rust
  pub fn write(self: &Self, args: &[BulkWriteArg]) -> Result<Vec<BulkResponse>, crate::Result> { /* ... */ }
  ```
  Writes values to multiple Data Layer nodes in a single synchronous bulk request.

- ```rust
  pub fn create(self: &Self, args: &[BulkCreateArg]) -> Result<Vec<BulkResponse>, crate::Result> { /* ... */ }
  ```
  Creates multiple Data Layer nodes in a single synchronous bulk request.

- ```rust
  pub fn delete(self: &Self, addresses: &[&str]) -> Result<Vec<BulkResponse>, crate::Result> { /* ... */ }
  ```
  Deletes multiple Data Layer nodes in a single synchronous bulk request.

- ```rust
  pub fn browse(self: &Self, addresses: &[&str]) -> Result<Vec<BulkResponse>, crate::Result> { /* ... */ }
  ```
  Browses multiple Data Layer nodes in a single synchronous bulk request.

- ```rust
  pub fn metadata(self: &Self, addresses: &[&str]) -> Result<Vec<BulkResponse>, crate::Result> { /* ... */ }
  ```
  Retrieves the metadata of multiple Data Layer nodes in a single synchronous bulk request.

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Drop**
  - ```rust
    fn drop(self: &mut Self) { /* ... */ }
    ```

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
## Module `async_bulk`

```rust
pub mod async_bulk { /* ... */ }
```

### Types

#### Struct `AsyncBulk`

Provides asynchronous bulk operations on the Data Layer.

`AsyncBulk` is created via [`Client::create_async_bulk`](crate::client::Client::create_async_bulk)
and allows reading, writing, browsing, and managing multiple nodes in a single round-trip.
Results are delivered via a [`super::ResponseCallback`] rather than a return value.

```rust
pub struct AsyncBulk {
    // Some fields omitted
}
```

##### Fields

| Name | Type | Documentation |
|------|------|---------------|
| *private fields* | ... | *Some fields have been omitted* |

##### Implementations

###### Methods

- ```rust
  pub fn read(self: &mut Self, args: &[BulkReadArg], callback: ResponseCallbackBox) -> Result<(), crate::Result> { /* ... */ }
  ```
  Asynchronously reads multiple Data Layer nodes.

- ```rust
  pub fn write(self: &mut Self, args: &[BulkWriteArg], callback: ResponseCallbackBox) -> Result<(), crate::Result> { /* ... */ }
  ```
  Asynchronously writes values to multiple Data Layer nodes.

- ```rust
  pub fn create(self: &mut Self, args: &[BulkCreateArg], callback: ResponseCallbackBox) -> Result<(), crate::Result> { /* ... */ }
  ```
  Asynchronously creates multiple Data Layer nodes.

- ```rust
  pub fn browse(self: &mut Self, addresses: &[&str], callback: ResponseCallbackBox) -> Result<(), crate::Result> { /* ... */ }
  ```
  Asynchronously browses multiple Data Layer addresses.

- ```rust
  pub fn metadata(self: &mut Self, addresses: &[&str], callback: ResponseCallbackBox) -> Result<(), crate::Result> { /* ... */ }
  ```
  Asynchronously retrieves metadata for multiple Data Layer nodes.

- ```rust
  pub fn delete(self: &mut Self, addresses: &[&str], callback: ResponseCallbackBox) -> Result<(), crate::Result> { /* ... */ }
  ```
  Asynchronously deletes multiple Data Layer nodes.

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Drop**
  - ```rust
    fn drop(self: &mut Self) { /* ... */ }
    ```

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
### Types

#### Struct `BulkResponse`

The response for a single entry in a bulk operation.

Each field corresponds to the outcome of one request sent in a bulk read.

```rust
pub struct BulkResponse {
    pub address: String,
    pub value: crate::Value,
    pub result: crate::Result,
    pub time: chrono::DateTime<chrono::Utc>,
}
```

##### Fields

| Name | Type | Documentation |
|------|------|---------------|
| `address` | `String` | The Data Layer address of the node that was queried. |
| `value` | `crate::Value` | The value returned by the node, or a default value if the request failed. |
| `result` | `crate::Result` | The operation result code for this individual request. |
| `time` | `chrono::DateTime<chrono::Utc>` | The server-side timestamp of when the response was recorded. |

##### Implementations

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
#### Struct `BulkReadArg`

Input argument for a single entry in a bulk read request.

```rust
pub struct BulkReadArg {
    pub address: String,
    pub argument: Option<crate::Value>,
}
```

##### Fields

| Name | Type | Documentation |
|------|------|---------------|
| `address` | `String` | The Data Layer address of the node to read. |
| `argument` | `Option<crate::Value>` | An optional argument value passed along with the read request.<br>Pass `None` if no argument is required. |

##### Implementations

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
#### Struct `BulkWriteArg`

Input argument for a single entry in a bulk write request.

```rust
pub struct BulkWriteArg {
    pub address: String,
    pub data: crate::Value,
}
```

##### Fields

| Name | Type | Documentation |
|------|------|---------------|
| `address` | `String` | The Data Layer address of the node to write. |
| `data` | `crate::Value` | An optional argument value passed along with the write request. |

##### Implementations

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
#### Struct `BulkCreateArg`

Input argument for a single entry in a bulk create request.

```rust
pub struct BulkCreateArg {
    pub address: String,
    pub data: Option<crate::Value>,
}
```

##### Fields

| Name | Type | Documentation |
|------|------|---------------|
| `address` | `String` | The Data Layer address of the node to create. |
| `data` | `Option<crate::Value>` | An optional argument value passed along with the create request.<br>Pass `None` if no argument is required. |

##### Implementations

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
#### Type Alias `ResponseCallbackBox`

Type alias for a boxed [`ResponseCallback`].

```rust
pub type ResponseCallbackBox = Box<dyn ResponseCallback>;
```

### Traits

#### Trait `ResponseCallback`

Callback trait for asynchronous bulk operation responses.

Implement this trait to receive the results of an async bulk request.
The `Send` bound is required because the callback is invoked from a C thread.
```ignore
struct MyCallback;
impl ResponseCallback for MyCallback {
    fn on_response(&mut self, response: Vec<BulkResponse>) {
        // Handle the bulk response here
    }
}
```

```rust
pub trait ResponseCallback: Send {
    /* Associated items */
}
```

##### Required Items

###### Required Methods

- `on_response`: Called by the Data Layer when all responses for a bulk operation are available.

## Module `subscription`

```rust
pub mod subscription { /* ... */ }
```

### Types

#### Struct `NotifyItem`

A single notification item delivered by a subscription callback.

Each item contains the updated node `data` and associated `info`
(address, timestamp, etc.) encoded as a FlatBuffers variant.

```rust
pub struct NotifyItem {
    pub data: crate::Value,
    // Some fields omitted
}
```

##### Fields

| Name | Type | Documentation |
|------|------|---------------|
| `data` | `crate::Value` | The updated value of the subscribed node. |
| *private fields* | ... | *Some fields have been omitted* |

##### Implementations

###### Methods

- ```rust
  pub fn address(self: &Self) -> &str { /* ... */ }
  ```
  Returns the Data Layer address of the updated node.

- ```rust
  pub fn timestamp(self: &Self) -> DateTime<Utc> { /* ... */ }
  ```
  Returns the timestamp of the update as a UTC `DateTime`.

- ```rust
  pub fn timestamp_raw(self: &Self) -> u64 { /* ... */ }
  ```
  Returns the raw FILETIME timestamp (100-nanosecond intervals since 1601-01-01 UTC).

- ```rust
  pub fn notify_type(self: &Self) -> NotifyType { /* ... */ }
  ```
  Returns the type of notification (e.g. `Data`, `Browse`, `Metadata`, `Keepalive`, `Event`, `Connection`).

- ```rust
  pub fn notify_type_str(self: &Self) -> &str { /* ... */ }
  ```
  Returns the type of notification (e.g. `Data`, `Browse`, `Metadata`, `Keepalive`, `Event`, `Connection`).

- ```rust
  pub fn event_type(self: &Self) -> Option<&str> { /* ... */ }
  ```
  Returns the event type string, if this notification was triggered by an event.

- ```rust
  pub fn sequence_number(self: &Self) -> u64 { /* ... */ }
  ```
  Returns the sequence number of the notification.

- ```rust
  pub fn source_name(self: &Self) -> Option<&str> { /* ... */ }
  ```
  Returns the source name of the event, if available.

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
#### Type Alias `SubscriptionCallbackBox`

Type alias for a boxed [`SubscriptionCallback`].

```rust
pub type SubscriptionCallbackBox = Box<dyn SubscriptionCallback>;
```

#### Struct `Subscription`

Represents an active Data Layer subscription.

A `Subscription` is created via
[`Client::create_subscription`](crate::client::Client::create_subscription).
It manages a set of subscribed nodes and delivers updates through a
[`SubscriptionCallback`]. The subscription is cleaned up when this value is dropped.

```rust
pub struct Subscription {
    // Some fields omitted
}
```

##### Fields

| Name | Type | Documentation |
|------|------|---------------|
| *private fields* | ... | *Some fields have been omitted* |

##### Implementations

###### Methods

- ```rust
  pub fn ruleset(self: &Self) -> &SubscriptionPropertiesT { /* ... */ }
  ```
  Returns the subscription ruleset (publish interval, keepalive, error interval, etc.).

- ```rust
  pub fn id(self: &Self) -> &str { /* ... */ }
  ```
  Returns the unique subscription ID from the ruleset.

- ```rust
  pub fn addresses(self: &Self) -> &[String] { /* ... */ }
  ```
  Returns the list of currently subscribed addresses.

- ```rust
  pub fn subscribe(self: &mut Self, addresses: &[&str]) -> Result<(), crate::Result> { /* ... */ }
  ```
  Adds multiple nodes to the subscription in a single call.

- ```rust
  pub fn unsubscribe(self: &mut Self, addresses: &[&str]) -> Result<(), crate::Result> { /* ... */ }
  ```
  Removes multiple nodes from the subscription in a single call.

- ```rust
  pub fn unsubscribe_all(self: &mut Self) -> Result<(), crate::Result> { /* ... */ }
  ```
  Removes all nodes from the subscription, deleting it completely.

- ```rust
  pub fn subscribe_modify(self: &mut Self, addresses: &[&str]) -> Result<(), crate::Result> { /* ... */ }
  ```
  Replaces all subscribed nodes with a new set of addresses.

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Drop**
  - ```rust
    fn drop(self: &mut Self) { /* ... */ }
    ```
    Removes the callback context from the global map when the subscription is dropped.

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
### Traits

#### Trait `SubscriptionCallback`

Callback trait for subscription notifications.

Implement this trait to receive updates whenever subscribed nodes change.
The `Send` bound is required because callbacks are invoked from a C thread.
Do **not** block inside `on_notify` — doing so prevents the C layer from
dispatching further callbacks or forwarding requests to the broker.
```ignore
struct MySubscriptionCallback;
impl SubscriptionCallback for MySubscriptionCallback {
    fn on_notify(&mut self, status: crate::Result, items: Vec<NotifyItem>) {
        if status == crate::Result::Ok {
            for item in items {
                println!("Received update for {}: {:?}", item.address(), item.data);
            }
        } else {
            eprintln!("Subscription error: {:?}", status);
        }
    }
}
let callback = Box::new(MySubscriptionCallback);
let subscription = client.create_subscription(ruleset, callback).expect("Failed to create subscription");
subscription.subscribe(&["my/node1", "my/node2"]).expect("Failed to subscribe to nodes");
```

```rust
pub trait SubscriptionCallback: Send {
    /* Associated items */
}
```

##### Required Items

###### Required Methods

- `on_notify`: Called by the Data Layer whenever subscribed node values are updated.

## Module `sync`

```rust
pub mod sync { /* ... */ }
```

### Types

#### Struct `SyncClient`

Provides synchronous access to the Data Layer.

A `SyncClient` is obtained via [`Client::sync`](crate::client::Client::sync).
It holds the same `DLR_CLIENT` handle as the owning [`Client`](crate::client::Client)
and is dropped automatically when the `Client` is dropped.

```rust
pub struct SyncClient {
    // Some fields omitted
}
```

##### Fields

| Name | Type | Documentation |
|------|------|---------------|
| *private fields* | ... | *Some fields have been omitted* |

##### Implementations

###### Methods

- ```rust
  pub fn ping(self: &Self) -> Result<(), crate::Result> { /* ... */ }
  ```
  Pings the Data Layer to check if the connection is alive.

- ```rust
  pub fn read(self: &Self, address: &str) -> Result<crate::Value, crate::Result> { /* ... */ }
  ```
  Reads the value at the given Data Layer `address`.

- ```rust
  pub fn read_arg(self: &Self, address: &str, arg: &crate::Value) -> Result<crate::Value, crate::Result> { /* ... */ }
  ```
  Reads the value at the given Data Layer `address`, passing `arg` as input argument.

- ```rust
  pub fn write(self: &Self, address: &str, arg: &crate::Value) -> Result<crate::Value, crate::Result> { /* ... */ }
  ```
  Writes `arg` to the given Data Layer `address`.

- ```rust
  pub fn browse(self: &Self, address: &str) -> Result<Vec<String>, crate::Result> { /* ... */ }
  ```
  Browses the child nodes at the given Data Layer `address`.

- ```rust
  pub fn create(self: &Self, address: &str, arg: &crate::Value) -> Result<crate::Value, crate::Result> { /* ... */ }
  ```
  Creates a new node at the given Data Layer `address` with the value `arg`.

- ```rust
  pub fn remove(self: &Self, address: &str) -> Result<(), crate::Result> { /* ... */ }
  ```
  Removes the node at the given Data Layer `address`.

- ```rust
  pub fn metadata(self: &Self, address: &str) -> Result<MetadataT, crate::Result> { /* ... */ }
  ```
  Reads the metadata of the node at the given Data Layer `address`.

- ```rust
  pub fn read_json(self: &Self, cv: &crate::Converter, address: &str, indent_step: i32, data: Option<&[u8]>) -> Result<crate::Value, crate::Result> { /* ... */ }
  ```
  Reads the value at the given Data Layer `address` and returns it as a JSON-encoded variant.

- ```rust
  pub fn write_json(self: &Self, cv: &crate::Converter, address: &str, json: &[u8]) -> Result<(), crate::converter::ConverterResult> { /* ... */ }
  ```
  Writes a JSON-encoded value to the given Data Layer `address`.

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
### Types

#### Struct `Client`

Represents a Data Layer client connection.

A `Client` is created via [`Factory::create_client`](crate::factory::Factory::create_client)
and provides read/write access to nodes in the Data Layer.

```rust
pub struct Client {
    // Some fields omitted
}
```

##### Fields

| Name | Type | Documentation |
|------|------|---------------|
| *private fields* | ... | *Some fields have been omitted* |

##### Implementations

###### Methods

- ```rust
  pub fn is_connected(self: &Self) -> bool { /* ... */ }
  ```
  Returns `true` if the client is currently connected to the Data Layer.

- ```rust
  pub fn update_timeout(self: &Self, timeout_setting: TimeoutSetting, timeout_ms: u32) -> Result<(), crate::Result> { /* ... */ }
  ```
  Sets the timeout for the given [`TimeoutSetting`] to `timeout_ms` milliseconds.

- ```rust
  pub fn sync(self: &Self) -> &SyncClient { /* ... */ }
  ```
  Returns a reference to the [`SyncClient`].

- ```rust
  pub fn async(self: &Self) -> &AsyncClient { /* ... */ }
  ```
  Returns a reference to the [`AsyncClient`].

- ```rust
  pub fn auth_token(self: &Self) -> String { /* ... */ }
  ```
  Returns the authentication token currently set on this client.

- ```rust
  pub fn update_auth_token(self: &Self, token: &str) { /* ... */ }
  ```
  Sets the authentication token used by this client for subsequent requests.

- ```rust
  pub fn connection_status(self: &Self) -> crate::Result { /* ... */ }
  ```
  Returns the current connection status of the client as a [`crate::Result`] code.

- ```rust
  pub fn create_subscription(self: &Self, ruleset: crate::Value, callback: crate::SubscriptionCallbackBox) -> Result<crate::Subscription, crate::Result> { /* ... */ }
  ```
  Creates a new subscription for Data Layer nodes matching the given ruleset.

- ```rust
  pub fn create_sync_bulk(self: &Self) -> crate::SyncBulk { /* ... */ }
  ```
  Creates a new synchronous bulk operation handle.

- ```rust
  pub fn create_async_bulk(self: &Self) -> crate::AsyncBulk { /* ... */ }
  ```
  Creates a new asynchronous bulk operation handle.

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Drop**
  - ```rust
    fn drop(self: &mut Self) { /* ... */ }
    ```
    Drops the `SyncClient` (if created) and deletes the native client handle.

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
## Module `provider`

```rust
pub mod provider { /* ... */ }
```

### Modules

## Module `node`

**Attributes:**

- `Other("#[allow(dead_code)]")`

```rust
pub mod node { /* ... */ }
```

### Types

#### Type Alias `NodeCallbackBox`

Type alias for a boxed [`NodeCallback`].

```rust
pub type NodeCallbackBox = Box<dyn NodeCallback>;
```

#### Enum `NodeEvent`

Events dispatched to a [`ProviderNodeCallback`] for each datalayer operation.

```rust
pub enum NodeEvent {
    Create(String, crate::Value, NodeCallbackBox),
    Remove(String, NodeCallbackBox),
    Browse(String, NodeCallbackBox),
    Read(String, crate::Value, NodeCallbackBox),
    Write(String, crate::Value, NodeCallbackBox),
    Metadata(String, NodeCallbackBox),
    Subscribe(crate::provider::subscription::NodeSubscriptionBox),
    Unsubscribe(crate::provider::subscription::NodeSubscriptionBox),
}
```

##### Variants

###### `Create`

Fields:

| Index | Type | Documentation |
|-------|------|---------------|
| 0 | `String` |  |
| 1 | `crate::Value` |  |
| 2 | `NodeCallbackBox` |  |

###### `Remove`

Fields:

| Index | Type | Documentation |
|-------|------|---------------|
| 0 | `String` |  |
| 1 | `NodeCallbackBox` |  |

###### `Browse`

Fields:

| Index | Type | Documentation |
|-------|------|---------------|
| 0 | `String` |  |
| 1 | `NodeCallbackBox` |  |

###### `Read`

Fields:

| Index | Type | Documentation |
|-------|------|---------------|
| 0 | `String` |  |
| 1 | `crate::Value` |  |
| 2 | `NodeCallbackBox` |  |

###### `Write`

Fields:

| Index | Type | Documentation |
|-------|------|---------------|
| 0 | `String` |  |
| 1 | `crate::Value` |  |
| 2 | `NodeCallbackBox` |  |

###### `Metadata`

Fields:

| Index | Type | Documentation |
|-------|------|---------------|
| 0 | `String` |  |
| 1 | `NodeCallbackBox` |  |

###### `Subscribe`

Fields:

| Index | Type | Documentation |
|-------|------|---------------|
| 0 | `crate::provider::subscription::NodeSubscriptionBox` |  |

###### `Unsubscribe`

Fields:

| Index | Type | Documentation |
|-------|------|---------------|
| 0 | `crate::provider::subscription::NodeSubscriptionBox` |  |

##### Implementations

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
#### Type Alias `ProviderNodeCallbackBox`

Type alias for a boxed [`ProviderNodeCallback`].

```rust
pub type ProviderNodeCallbackBox = Box<dyn ProviderNodeCallback>;
```

#### Struct `ProviderNode`

A registered datalayer provider node backed by the C runtime.

Create a `ProviderNode` via [`ProviderNode::new`] and register it with a
[`Provider`](crate::provider::Provider) to start handling datalayer requests.

```rust
pub struct ProviderNode {
    // Some fields omitted
}
```

##### Fields

| Name | Type | Documentation |
|------|------|---------------|
| *private fields* | ... | *Some fields have been omitted* |

##### Implementations

###### Methods

- ```rust
  pub fn new(callback: ProviderNodeCallbackBox, subscription: bool) -> Self { /* ... */ }
  ```
  Creates a new [`ProviderNode`] and registers the given `callback` handler.

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Drop**
  - ```rust
    fn drop(self: &mut Self) { /* ... */ }
    ```

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
### Traits

#### Trait `NodeCallback`

Trait for responding to a datalayer node request from the C runtime.

```rust
pub trait NodeCallback: Send {
    /* Associated items */
}
```

##### Required Items

###### Required Methods

- `callback`

#### Trait `ProviderNodeCallback`

Implement this trait to handle datalayer node events.
The `Send` bound is required because the callback is invoked from a C thread.
Each method receives a [`NodeEvent`] with the relevant data and a callback handler to respond to the datalayer runtime.
```ignore
fn on_response(&mut self, event: NodeEvent) {
    match event {
        NodeEvent::Read(addr, data, callback) => {
            // handle read event and call callback.callback(result, value)
            let value = crate::Value::new(42); // example value
            callback.callback(ctrlx_datalayer::Result::Ok, &value);
        }
        NodeEvent::Write(addr, data, callback) => {
            // handle write event and call callback.callback(result, value)
           callback.callback(ctrlx_datalayer::Result::Ok, &data); // echo back written value
        }
        // handle other event types...
    }
}

```rust
pub trait ProviderNodeCallback: Send {
    /* Associated items */
}
```

##### Required Items

###### Required Methods

- `on_response`

### Functions

#### Function `on_create`

C callback invoked by the datalayer runtime when a node is created.

# Safety
`address` must be a valid NUL-terminated C string.

```rust
pub unsafe extern "C" fn on_create(user_data: *mut std::ffi::c_void, address: *const std::os::raw::c_char, data: *mut ::std::os::raw::c_void, callback: ::std::option::Option<unsafe extern "C" fn(*mut ::std::os::raw::c_void, ::std::os::raw::c_uint, *mut ::std::os::raw::c_void)>, callbackdata: *mut ::std::os::raw::c_void) -> ::std::os::raw::c_uint { /* ... */ }
```

#### Function `on_remove`

C callback invoked by the datalayer runtime when a node is removed.

# Safety
`address` must be a valid NUL-terminated C string.

```rust
pub unsafe extern "C" fn on_remove(user_data: *mut std::ffi::c_void, address: *const std::os::raw::c_char, callback: ::std::option::Option<unsafe extern "C" fn(*mut ::std::os::raw::c_void, ::std::os::raw::c_uint, *mut ::std::os::raw::c_void)>, callbackdata: *mut ::std::os::raw::c_void) -> ::std::os::raw::c_uint { /* ... */ }
```

#### Function `on_browse`

C callback invoked by the datalayer runtime when a node's children are browsed.

# Safety
`address` must be a valid NUL-terminated C string.

```rust
pub unsafe extern "C" fn on_browse(user_data: *mut std::ffi::c_void, address: *const std::os::raw::c_char, callback: ::std::option::Option<unsafe extern "C" fn(*mut ::std::os::raw::c_void, ::std::os::raw::c_uint, *mut ::std::os::raw::c_void)>, callbackdata: *mut ::std::os::raw::c_void) -> ::std::os::raw::c_uint { /* ... */ }
```

#### Function `on_read`

C callback invoked by the datalayer runtime when a node's value is read.

# Safety
`address` must be a valid NUL-terminated C string.

```rust
pub unsafe extern "C" fn on_read(user_data: *mut std::ffi::c_void, address: *const std::os::raw::c_char, data: *mut ::std::os::raw::c_void, callback: ::std::option::Option<unsafe extern "C" fn(*mut ::std::os::raw::c_void, ::std::os::raw::c_uint, *mut ::std::os::raw::c_void)>, callbackdata: *mut ::std::os::raw::c_void) -> ::std::os::raw::c_uint { /* ... */ }
```

#### Function `on_write`

C callback invoked by the datalayer runtime when a node's value is written.

# Safety
`address` must be a valid NUL-terminated C string.

```rust
pub unsafe extern "C" fn on_write(user_data: *mut std::ffi::c_void, address: *const std::os::raw::c_char, data: *mut ::std::os::raw::c_void, callback: ::std::option::Option<unsafe extern "C" fn(*mut ::std::os::raw::c_void, ::std::os::raw::c_uint, *mut ::std::os::raw::c_void)>, callbackdata: *mut ::std::os::raw::c_void) -> ::std::os::raw::c_uint { /* ... */ }
```

#### Function `on_metadata`

C callback invoked by the datalayer runtime when a node's metadata is requested.

# Safety
`address` must be a valid NUL-terminated C string.

```rust
pub unsafe extern "C" fn on_metadata(user_data: *mut std::ffi::c_void, address: *const std::os::raw::c_char, callback: ::std::option::Option<unsafe extern "C" fn(*mut ::std::os::raw::c_void, ::std::os::raw::c_uint, *mut ::std::os::raw::c_void)>, callbackdata: *mut ::std::os::raw::c_void) -> ::std::os::raw::c_uint { /* ... */ }
```

#### Function `on_subscribe`

C callback invoked by the datalayer runtime when a client subscribes to a node.

# Safety
`address` must be a valid NUL-terminated C string.

```rust
pub unsafe extern "C" fn on_subscribe(user_data: *mut std::ffi::c_void, subscription: *mut ::std::os::raw::c_void, address: *const std::os::raw::c_char) -> ::std::os::raw::c_uint { /* ... */ }
```

#### Function `on_unsubscribe`

C callback invoked by the datalayer runtime when a client unsubscribes from a node.

# Safety
`address` must be a valid NUL-terminated C string.

```rust
pub unsafe extern "C" fn on_unsubscribe(user_data: *mut std::ffi::c_void, subscription: *mut ::std::os::raw::c_void, address: *const std::os::raw::c_char) -> ::std::os::raw::c_uint { /* ... */ }
```

## Module `subscription`

```rust
pub mod subscription { /* ... */ }
```

### Types

#### Type Alias `NodeSubscriptionBox`

Type alias for a boxed [`NodeSubscription`].

```rust
pub type NodeSubscriptionBox = Box<dyn NodeSubscription>;
```

#### Struct `NotifyItemPublish`

A single item to be published to a subscription.

Holds the node's data value together with its notification metadata
(address, timestamp, notify type, …).

```rust
pub struct NotifyItemPublish {
    // Some fields omitted
}
```

##### Fields

| Name | Type | Documentation |
|------|------|---------------|
| *private fields* | ... | *Some fields have been omitted* |

##### Implementations

###### Methods

- ```rust
  pub fn new(address: String) -> Self { /* ... */ }
  ```
  Creates a new [`NotifyItemPublish`] for the given node `address`.

- ```rust
  pub fn data(self: &Self) -> &Value { /* ... */ }
  ```
  Returns a shared reference to the node's data value.

- ```rust
  pub fn data_mut(self: &mut Self) -> &mut Value { /* ... */ }
  ```
  Returns a mutable reference to the node's data value.

- ```rust
  pub fn info(self: &Self) -> Value { /* ... */ }
  ```
  Serializes the notification metadata into a Flatbuffers-encoded [`Value`].

- ```rust
  pub fn timestamp_mut(self: &mut Self, timestamp: chrono::DateTime<chrono::Utc>) { /* ... */ }
  ```
  Sets the notification timestamp (converted from UTC to FILETIME internally).

- ```rust
  pub fn notify_type_mut(self: &mut Self, notify_type: NotifyType) { /* ... */ }
  ```
  Sets the notification type (e.g. `Data`, `Event`, …).

- ```rust
  pub fn event_type_mut(self: &mut Self, event_type: String) { /* ... */ }
  ```
  Sets an optional event-type string for event notifications.

- ```rust
  pub fn sequence_mut(self: &mut Self, sequence: u64) { /* ... */ }
  ```
  Sets the monotonically increasing sequence number for ordering notifications.

- ```rust
  pub fn source_mut(self: &mut Self, source: String) { /* ... */ }
  ```
  Sets the source name that identifies the origin of the notification.

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
### Traits

#### Trait `NodeSubscription`

Trait representing an active subscription on a provider node.

`Send + Sync` so implementors can be wrapped in `Arc` and called from any thread.
The lifetime of the underlying handle is managed by the datalayer runtime:
it is valid between the `Subscribe` and the corresponding `Unsubscribe` event.

```rust
pub trait NodeSubscription: Send + Sync {
    /* Associated items */
}
```

##### Required Items

###### Required Methods

- `address`: The subscribed node address.
- `unique_id`: Unique handle ID of the underlying `DLR_SUBSCRIPTION`.
- `nodes`: All node addresses currently registered with this subscription.
- `datetime_utc`: Timestamp of the last published value as UTC `DateTime`.
- `properties`: Subscription properties (id, intervals, …).
- `publish`: Publishes updated values for the subscribed nodes.

### Types

#### Struct `Provider`

Represents a Data Layer provider.

A `Provider` is created via [`Factory::create_provider`](crate::factory::Factory::create_provider)
and allows publishing nodes and handling read/write/browse requests from clients.

```rust
pub struct Provider {
    // Some fields omitted
}
```

##### Fields

| Name | Type | Documentation |
|------|------|---------------|
| *private fields* | ... | *Some fields have been omitted* |

##### Implementations

###### Methods

- ```rust
  pub fn start(self: &Self) -> Result<(), crate::Result> { /* ... */ }
  ```
  Starts the provider and begins accepting client requests.

- ```rust
  pub fn stop(self: &Self) -> Result<(), crate::Result> { /* ... */ }
  ```
  Stops the provider and rejects any further client requests.

- ```rust
  pub fn is_connected(self: &Self) -> bool { /* ... */ }
  ```
  Returns `true` if the provider is currently connected to the datalayer broker.

- ```rust
  pub fn register_node(self: &Self, address: &str, node: &node::ProviderNode) -> Result<(), crate::Result> { /* ... */ }
  ```
  Registers a [`ProviderNode`] under the given datalayer `address`.

- ```rust
  pub fn unregister_node(self: &Self, address: &str) -> Result<(), crate::Result> { /* ... */ }
  ```
  Unregisters the node previously registered under the given datalayer `address`.

- ```rust
  pub fn register_type(self: &Self, address: &str, pathname: &str) -> Result<(), crate::Result> { /* ... */ }
  ```
  Registers a FlatBuffers type definition stored at `pathname` under the given datalayer `address`.

- ```rust
  pub fn unregister_type(self: &Self, address: &str) -> Result<(), crate::Result> { /* ... */ }
  ```
  Unregisters the type previously registered under the given datalayer `address`.

- ```rust
  pub fn register_type_variant(self: &Self, address: &str, value: &crate::Value) -> Result<(), crate::Result> { /* ... */ }
  ```
  Registers a type definition supplied as a [`crate::Value`] variant under the given datalayer `address`.

- ```rust
  pub fn registered_type(self: &Self, address: &str) -> Result<crate::variant::Value, crate::Result> { /* ... */ }
  ```
  Returns all type definitions registered under the given datalayer `address`.

- ```rust
  pub fn token(self: &Self) -> crate::variant::Value { /* ... */ }
  ```
  Returns the security token associated with this provider.

- ```rust
  pub fn registered_node_paths(self: &Self) -> Result<crate::variant::Value, crate::Result> { /* ... */ }
  ```
  Returns the datalayer paths of all nodes successfully registered by this provider.

- ```rust
  pub fn rejected_node_paths(self: &Self) -> Result<crate::variant::Value, crate::Result> { /* ... */ }
  ```
  Returns the datalayer paths of all nodes whose registration was rejected by the broker.

- ```rust
  pub fn update_timeout_node(self: &Self, node: &ProviderNode, timeout: u32) -> Result<(), crate::Result> { /* ... */ }
  ```
  Sets the response timeout (in milliseconds) for the given [`ProviderNode`].

- ```rust
  pub fn publish_event(self: &Self, data: &crate::Value, event_info: &crate::Value) -> Result<(), crate::Result> { /* ... */ }
  ```
  Publishes an event to the datalayer.

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Drop**
  - ```rust
    fn drop(self: &mut Self) { /* ... */ }
    ```
    Deletes the native provider handle when the `Provider` is dropped.

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
## Module `variant`

```rust
pub mod variant { /* ... */ }
```

### Modules

## Module `value`

```rust
pub mod value { /* ... */ }
```

### Types

#### Struct `Value`

A type-safe value container for ctrlX Data Layer variants.

Stores an arbitrary value together with its [`VariantType`].
The type is automatically derived from the Rust type when creating a value
and can be overridden afterwards using setter methods.

# Example

```
use ctrlx_datalayer::variant::Value;
use ctrlx_datalayer::variant::VariantType;

let v = Value::new(42i32);
assert_eq!(v.variant_type(), VariantType::Int32);
assert_eq!(v.as_ref::<i32>(), Some(&42));
assert_eq!(v.as_ref::<i32>().copied().unwrap(), 42i32);
```
Hint: For vec! macros or String, you must use 'cloned' to get the inner values, e.g. `v.as_ref::<Vec<i32>>().cloned().unwrap()`

```rust
pub struct Value {
    // Some fields omitted
}
```

##### Fields

| Name | Type | Documentation |
|------|------|---------------|
| *private fields* | ... | *Some fields have been omitted* |

##### Implementations

###### Methods

- ```rust
  pub fn new<T: ''static + HasVariantType>(val: T) -> Self { /* ... */ }
  ```
  Creates a new `Value` with the given value.

- ```rust
  pub fn new_default() -> Self { /* ... */ }
  ```
  Creates a new `Value` with an unknown type and a placeholder value.

- ```rust
  pub fn variant_type(self: &Self) -> VariantType { /* ... */ }
  ```
  Returns the current [`VariantType`] of the stored value.

- ```rust
  pub fn as_ref<T: ''static>(self: &Self) -> Option<&T> { /* ... */ }
  ```
  Returns a reference to the stored value if the type `T` matches.

- ```rust
  pub fn bool8_mut(self: &mut Self, val: bool) { /* ... */ }
  ```
  Sets the value to a `bool` (type: [`VariantType::Bool8`]).

- ```rust
  pub fn int8_mut(self: &mut Self, val: i8) { /* ... */ }
  ```
  Sets the value to an `i8` (type: [`VariantType::Int8`]).

- ```rust
  pub fn uint8_mut(self: &mut Self, val: u8) { /* ... */ }
  ```
  Sets the value to a `u8` (type: [`VariantType::Uint8`]).

- ```rust
  pub fn int16_mut(self: &mut Self, val: i16) { /* ... */ }
  ```
  Sets the value to an `i16` (type: [`VariantType::Int16`]).

- ```rust
  pub fn uint16_mut(self: &mut Self, val: u16) { /* ... */ }
  ```
  Sets the value to a `u16` (type: [`VariantType::Uint16`]).

- ```rust
  pub fn int32_mut(self: &mut Self, val: i32) { /* ... */ }
  ```
  Sets the value to an `i32` (type: [`VariantType::Int32`]).

- ```rust
  pub fn uint32_mut(self: &mut Self, val: u32) { /* ... */ }
  ```
  Sets the value to a `u32` (type: [`VariantType::Uint32`]).

- ```rust
  pub fn int64_mut(self: &mut Self, val: i64) { /* ... */ }
  ```
  Sets the value to an `i64` (type: [`VariantType::Int64`]).

- ```rust
  pub fn uint64_mut(self: &mut Self, val: u64) { /* ... */ }
  ```
  Sets the value to a `u64` (type: [`VariantType::Uint64`]).

- ```rust
  pub fn float32_mut(self: &mut Self, val: f32) { /* ... */ }
  ```
  Sets the value to an `f32` (type: [`VariantType::Float32`]).

- ```rust
  pub fn float64_mut(self: &mut Self, val: f64) { /* ... */ }
  ```
  Sets the value to an `f64` (type: [`VariantType::Float64`]).

- ```rust
  pub fn string_mut(self: &mut Self, val: String) { /* ... */ }
  ```
  Sets the value to a `String` (type: [`VariantType::String`]).

- ```rust
  pub fn vec_bool8_mut(self: &mut Self, val: Vec<bool>) { /* ... */ }
  ```
  Sets the value to a `Vec<bool>` (type: [`VariantType::ArrayOfBool8`]).

- ```rust
  pub fn vec_int8_mut(self: &mut Self, val: Vec<i8>) { /* ... */ }
  ```
  Sets the value to a `Vec<i8>` (type: [`VariantType::ArrayOfInt8`]).

- ```rust
  pub fn vec_int16_mut(self: &mut Self, val: Vec<i16>) { /* ... */ }
  ```
  Sets the value to a `Vec<i16>` (type: [`VariantType::ArrayOfInt16`]).

- ```rust
  pub fn vec_int32_mut(self: &mut Self, val: Vec<i32>) { /* ... */ }
  ```
  Sets the value to a `Vec<i32>` (type: [`VariantType::ArrayOfInt32`]).

- ```rust
  pub fn vec_int64_mut(self: &mut Self, val: Vec<i64>) { /* ... */ }
  ```
  Sets the value to a `Vec<i64>` (type: [`VariantType::ArrayOfInt64`]).

- ```rust
  pub fn vec_uint8_mut(self: &mut Self, val: Vec<u8>) { /* ... */ }
  ```
  Sets the value to a `Vec<u8>` (type: [`VariantType::ArrayOfUint8`]).

- ```rust
  pub fn vec_uint16_mut(self: &mut Self, val: Vec<u16>) { /* ... */ }
  ```
  Sets the value to a `Vec<u16>` (type: [`VariantType::ArrayOfUint16`]).

- ```rust
  pub fn vec_uint32_mut(self: &mut Self, val: Vec<u32>) { /* ... */ }
  ```
  Sets the value to a `Vec<u32>` (type: [`VariantType::ArrayOfUint32`]).

- ```rust
  pub fn vec_uint64_mut(self: &mut Self, val: Vec<u64>) { /* ... */ }
  ```
  Sets the value to a `Vec<u64>` (type: [`VariantType::ArrayOfUint64`]).

- ```rust
  pub fn vec_float32_mut(self: &mut Self, val: Vec<f32>) { /* ... */ }
  ```
  Sets the value to a `Vec<f32>` (type: [`VariantType::ArrayOfFloat32`]).

- ```rust
  pub fn vec_float64_mut(self: &mut Self, val: Vec<f64>) { /* ... */ }
  ```
  Sets the value to a `Vec<f64>` (type: [`VariantType::ArrayOfFloat64`]).

- ```rust
  pub fn vec_string_mut(self: &mut Self, val: Vec<String>) { /* ... */ }
  ```
  Sets the value to a `Vec<String>` (type: [`VariantType::ArrayOfString`]).

- ```rust
  pub fn timestamp_mut(self: &mut Self, val: DateTime<Utc>) { /* ... */ }
  ```
  Sets the value to a [`DateTime<Utc>`] (type: [`VariantType::Timestamp`]).

- ```rust
  pub fn vec_timestamp_mut(self: &mut Self, val: Vec<DateTime<Utc>>) { /* ... */ }
  ```
  Sets the value to a `Vec<DateTime<Utc>>` (type: [`VariantType::ArrayOfTimestamp`]).

- ```rust
  pub fn flatbuffers_mut(self: &mut Self, val: Vec<u8>) { /* ... */ }
  ```
  Sets the value to a `Vec<u8>` (type: [`VariantType::Flatbuffers`]).

- ```rust
  pub fn raw_mut(self: &mut Self, val: Vec<u8>) { /* ... */ }
  ```
  Sets the value to a `Vec<u8>` (type: [`VariantType::Raw`]).    

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

  - ```rust
    fn try_from(info: &NotifyInfoT) -> Result<Self, <Self as >::Error> { /* ... */ }
    ```

  - ```rust
    fn try_from(value: &crate::Value) -> Result<Self, <Self as >::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
### Traits

#### Trait `HasVariantType`

Maps a Rust type to its corresponding [`VariantType`].

This trait is used by [`Value::new`] to automatically set the correct
[`VariantType`] when creating a value.

```rust
pub trait HasVariantType {
    /* Associated items */
}
```

> This trait is not object-safe and cannot be used in dynamic trait objects.

##### Required Items

###### Required Methods

- `variant_type`: Returns the [`VariantType`] that corresponds to this Rust type.

##### Implementations

This trait is implemented for the following types:

- `bool`
- `i8`
- `u8`
- `i16`
- `u16`
- `i32`
- `u32`
- `i64`
- `u64`
- `f32`
- `f64`
- `String`
- `chrono::DateTime<chrono::Utc>`
- `Vec<bool>`
- `Vec<i8>`
- `Vec<u8>`
- `Vec<i16>`
- `Vec<u16>`
- `Vec<i32>`
- `Vec<u32>`
- `Vec<i64>`
- `Vec<u64>`
- `Vec<f32>`
- `Vec<f64>`
- `Vec<String>`
- `Vec<chrono::DateTime<chrono::Utc>>`

## Module `variant_type`

Variant type codes for the ctrlX Data Layer.

These constants correspond to the `DLR_VARIANT_TYPE_DLR_VARIANT_TYPE_*` values
defined in the C API (`comm/datalayer/c/comm_datalayer.h`).

```rust
pub mod variant_type { /* ... */ }
```

### Types

#### Enum `VariantType`

Represents the Datalayer Variant Type

```rust
pub enum VariantType {
    Unknown,
    Bool8,
    Int8,
    Uint8,
    Int16,
    Uint16,
    Int32,
    Uint32,
    Int64,
    Uint64,
    Float32,
    Float64,
    String,
    ArrayOfBool8,
    ArrayOfInt8,
    ArrayOfUint8,
    ArrayOfInt16,
    ArrayOfUint16,
    ArrayOfInt32,
    ArrayOfUint32,
    ArrayOfInt64,
    ArrayOfUint64,
    ArrayOfFloat32,
    ArrayOfFloat64,
    ArrayOfString,
    Raw,
    Flatbuffers,
    Timestamp,
    ArrayOfTimestamp,
    UnknownCode(std::os::raw::c_uint),
}
```

##### Variants

###### `Unknown`

Unknown data type.

###### `Bool8`

Bool 8 bit.

###### `Int8`

Signed int 8 bit.

###### `Uint8`

Unsigned int 8 bit.

###### `Int16`

Signed int 16 bit.

###### `Uint16`

Unsigned int 16 bit.

###### `Int32`

Signed int 32 bit.

###### `Uint32`

Unsigned int 32 bit.

###### `Int64`

Signed int 64 bit.

###### `Uint64`

Unsigned int 64 bit.

###### `Float32`

Float 32 bit.

###### `Float64`

Float 64 bit.

###### `String`

String (UTF-8).

###### `ArrayOfBool8`

Array of bool 8 bit.

###### `ArrayOfInt8`

Array of signed int 8 bit.

###### `ArrayOfUint8`

Array of unsigned int 8 bit.

###### `ArrayOfInt16`

Array of signed int 16 bit.

###### `ArrayOfUint16`

Array of unsigned int 16 bit.

###### `ArrayOfInt32`

Array of signed int 32 bit.

###### `ArrayOfUint32`

Array of unsigned int 32 bit.

###### `ArrayOfInt64`

Array of signed int 64 bit.

###### `ArrayOfUint64`

Array of unsigned int 64 bit.

###### `ArrayOfFloat32`

Array of float 32 bit.

###### `ArrayOfFloat64`

Array of float 64 bit.

###### `ArrayOfString`

Array of string (UTF-8).

###### `Raw`

Raw bytes.

###### `Flatbuffers`

Bytes as a complex data type encoded as a FlatBuffer.

###### `Timestamp`

Timestamp (FILETIME) 64 bit, 100 ns since 1.1.1601 (UTC).

###### `ArrayOfTimestamp`

Array of timestamps.

###### `UnknownCode`

Used if the C library returns an unknown type code.

Fields:

| Index | Type | Documentation |
|-------|------|---------------|
| 0 | `std::os::raw::c_uint` |  |

##### Implementations

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Clone**
  - ```rust
    fn clone(self: &Self) -> VariantType { /* ... */ }
    ```

- **CloneToUninit**
  - ```rust
    unsafe fn clone_to_uninit(self: &Self, dest: *mut u8) { /* ... */ }
    ```

- **Copy**
- **Debug**
  - ```rust
    fn fmt(self: &Self, f: &mut $crate::fmt::Formatter<''_>) -> $crate::fmt::Result { /* ... */ }
    ```

- **Display**
  - ```rust
    fn fmt(self: &Self, f: &mut fmt::Formatter<''_>) -> fmt::Result { /* ... */ }
    ```

- **Eq**
- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

  - ```rust
    fn from(type_code: std::os::raw::c_uint) -> Self { /* ... */ }
    ```

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **PartialEq**
  - ```rust
    fn eq(self: &Self, other: &VariantType) -> bool { /* ... */ }
    ```

- **RefUnwindSafe**
- **Send**
- **StructuralPartialEq**
- **Sync**
- **ToOwned**
  - ```rust
    fn to_owned(self: &Self) -> T { /* ... */ }
    ```

  - ```rust
    fn clone_into(self: &Self, target: &mut T) { /* ... */ }
    ```

- **ToString**
  - ```rust
    fn to_string(self: &Self) -> String { /* ... */ }
    ```

- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
### Re-exports

#### Re-export `Value`

```rust
pub use value::Value;
```

#### Re-export `VariantType`

```rust
pub use variant_type::VariantType;
```

## Module `converter`

```rust
pub mod converter { /* ... */ }
```

### Types

#### Struct `Converter`

Provides JSON conversion capabilities for Data Layer variant values.

The `Converter` is obtained from a [`System`](crate::system::System) and wraps
the native JSON converter handle. It can be used to serialize and deserialize
Data Layer variants to and from their JSON representation.

```rust
pub struct Converter {
    // Some fields omitted
}
```

##### Fields

| Name | Type | Documentation |
|------|------|---------------|
| *private fields* | ... | *Some fields have been omitted* |

##### Implementations

###### Methods

- ```rust
  pub fn schema(self: &Self, schema: Schema) -> Result<Vec<u8>, crate::Result> { /* ... */ }
  ```
  Returns the FlatBuffers-encoded schema for the given [`Schema`] type.

- ```rust
  pub fn generate_json_simple(self: &Self, data: &crate::Value, indent_step: i32) -> Result<Vec<u8>, crate::Result> { /* ... */ }
  ```
  Serializes a simple (scalar) Data Layer variant to its JSON byte representation.

- ```rust
  pub fn parse_json_simple(self: &Self, json: &str) -> Result<crate::Value, ConverterResult> { /* ... */ }
  ```
  Deserializes a simple (scalar) JSON string into a Data Layer variant value.

- ```rust
  pub fn parse_json_simple_vec(self: &Self, json: &[u8]) -> Result<crate::Value, ConverterResult> { /* ... */ }
  ```
  Deserializes a simple (scalar) JSON byte slice into a Data Layer variant value.

- ```rust
  pub fn generate_json_complex(self: &Self, data: Vec<u8>, ty: Vec<u8>, indent_step: i32) -> Result<Vec<u8>, crate::Result> { /* ... */ }
  ```
  Serializes a complex (FlatBuffers-encoded) Data Layer variant together with its type

- ```rust
  pub fn parse_json_complex(self: &Self, json: Vec<u8>, ty: Vec<u8>) -> Result<Vec<u8>, ConverterResult> { /* ... */ }
  ```
  Deserializes a complex JSON byte vector using the provided FlatBuffers type descriptor

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Drop**
  - ```rust
    fn drop(self: &mut Self) { /* ... */ }
    ```
    Resets the native converter handle when the `Converter` is dropped.

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
#### Enum `ConverterResult`

Represents errors that can occur during converter operations.

```rust
pub enum ConverterResult {
    GetSchemaFailed(crate::Result),
    GenerateJsonSimpleFailed(crate::Result),
    ParseJsonSimpleFailed(crate::Result, String),
    GenerateJsonComplexFailed(crate::Result),
    ParseJsonComplexFailed(crate::Result, String),
    CommonFailed(crate::Result, String),
}
```

##### Variants

###### `GetSchemaFailed`

Retrieving the schema for a given type failed.

Fields:

| Index | Type | Documentation |
|-------|------|---------------|
| 0 | `crate::Result` |  |

###### `GenerateJsonSimpleFailed`

Generating a simple JSON representation from a variant failed.

Fields:

| Index | Type | Documentation |
|-------|------|---------------|
| 0 | `crate::Result` |  |

###### `ParseJsonSimpleFailed`

Parsing a simple JSON string into a variant failed. Contains the error result and an error message.

Fields:

| Index | Type | Documentation |
|-------|------|---------------|
| 0 | `crate::Result` |  |
| 1 | `String` |  |

###### `GenerateJsonComplexFailed`

Generating a complex JSON representation from a variant with type information failed.

Fields:

| Index | Type | Documentation |
|-------|------|---------------|
| 0 | `crate::Result` |  |

###### `ParseJsonComplexFailed`

Parsing a complex JSON string with type information into a variant failed. Contains the error result and an error message.

Fields:

| Index | Type | Documentation |
|-------|------|---------------|
| 0 | `crate::Result` |  |
| 1 | `String` |  |

###### `CommonFailed`

Fields:

| Index | Type | Documentation |
|-------|------|---------------|
| 0 | `crate::Result` |  |
| 1 | `String` |  |

##### Implementations

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Debug**
  - ```rust
    fn fmt(self: &Self, f: &mut $crate::fmt::Formatter<''_>) -> $crate::fmt::Result { /* ... */ }
    ```

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
## Module `timeout_settings`

```rust
pub mod timeout_settings { /* ... */ }
```

### Types

#### Enum `TimeoutSetting`

**Attributes:**

- `Repr(AttributeRepr { kind: Rust, align: None, packed: None, int: Some("u32") })`

Represents the different timeout settings for a DataLayer connection.

```rust
pub enum TimeoutSetting {
    Idle = 0,
    Ping = 1,
    Reconnect = 2,
}
```

##### Variants

###### `Idle`

Timeout for idle connections.

Discriminant: `0`

Discriminant value: `0`

###### `Ping`

Timeout for ping messages.

Discriminant: `1`

Discriminant value: `1`

###### `Reconnect`

Timeout for reconnect attempts.

Discriminant: `2`

Discriminant value: `2`

##### Implementations

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Clone**
  - ```rust
    fn clone(self: &Self) -> TimeoutSetting { /* ... */ }
    ```

- **CloneToUninit**
  - ```rust
    unsafe fn clone_to_uninit(self: &Self, dest: *mut u8) { /* ... */ }
    ```

- **Copy**
- **Debug**
  - ```rust
    fn fmt(self: &Self, f: &mut $crate::fmt::Formatter<''_>) -> $crate::fmt::Result { /* ... */ }
    ```

- **Display**
  - ```rust
    fn fmt(self: &Self, f: &mut std::fmt::Formatter<''_>) -> std::fmt::Result { /* ... */ }
    ```
    Formats the `TimeoutSetting` as a human-readable string.

- **Eq**
- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

  - ```rust
    fn from(setting: TimeoutSetting) -> Self { /* ... */ }
    ```
    Converts a `TimeoutSetting` to a `u32`.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **PartialEq**
  - ```rust
    fn eq(self: &Self, other: &TimeoutSetting) -> bool { /* ... */ }
    ```

- **RefUnwindSafe**
- **Send**
- **StructuralPartialEq**
- **Sync**
- **ToOwned**
  - ```rust
    fn to_owned(self: &Self) -> T { /* ... */ }
    ```

  - ```rust
    fn clone_into(self: &Self, target: &mut T) { /* ... */ }
    ```

- **ToString**
  - ```rust
    fn to_string(self: &Self) -> String { /* ... */ }
    ```

- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

  - ```rust
    fn try_from(value: u32) -> Result<Self, <Self as >::Error> { /* ... */ }
    ```
    Tries to convert a `u32` to a `TimeoutSetting`.

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
## Module `connection_builder`

```rust
pub mod connection_builder { /* ... */ }
```

### Types

#### Struct `ConnectionString`

A newtype wrapper representing a datalayer connection string.

```rust
pub struct ConnectionString(/* private field */);
```

##### Fields

| Index | Type | Documentation |
|-------|------|---------------|
| 0 | `private` | *Private field* |

##### Implementations

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Clone**
  - ```rust
    fn clone(self: &Self) -> ConnectionString { /* ... */ }
    ```

- **CloneToUninit**
  - ```rust
    unsafe fn clone_to_uninit(self: &Self, dest: *mut u8) { /* ... */ }
    ```

- **Debug**
  - ```rust
    fn fmt(self: &Self, f: &mut $crate::fmt::Formatter<''_>) -> $crate::fmt::Result { /* ... */ }
    ```

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

  - ```rust
    fn from(c: ConnectionString) -> String { /* ... */ }
    ```
    Converts a [`ConnectionString`] into the inner [`String`] by consuming the wrapper.

  - ```rust
    fn from(c: &ConnectionString) -> String { /* ... */ }
    ```
    Converts a reference to a [`ConnectionString`] into a [`String`] by cloning the inner value.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **PartialEq**
  - ```rust
    fn eq(self: &Self, other: &ConnectionString) -> bool { /* ... */ }
    ```

- **RefUnwindSafe**
- **Send**
- **StructuralPartialEq**
- **Sync**
- **ToOwned**
  - ```rust
    fn to_owned(self: &Self) -> T { /* ... */ }
    ```

  - ```rust
    fn clone_into(self: &Self, target: &mut T) { /* ... */ }
    ```

- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
#### Struct `ConnectionBuilder`

Builder for creating a datalayer connection string.

Provides a fluent interface to configure the target address, credentials,
and SSL port before constructing the final [`ConnectionString`].
```ignore
let connection_string = ConnectionBuilder::new()
    .with_address("192.168.0.100".to_string())
    .with_user("admin".to_string())
    .with_password("xxx".to_string())
    .with_sslport(8443)
    .build();
```

```rust
pub struct ConnectionBuilder {
    // Some fields omitted
}
```

##### Fields

| Name | Type | Documentation |
|------|------|---------------|
| *private fields* | ... | *Some fields have been omitted* |

##### Implementations

###### Methods

- ```rust
  pub fn new() -> Self { /* ... */ }
  ```
  Creates a new `ConnectionBuilder` with default values:

- ```rust
  pub fn with_ip_virtual(self: &mut Self) -> &mut Self { /* ... */ }
  ```
  Configures the builder for a virtual machine (QEMU) environment.

- ```rust
  pub fn with_address(self: &mut Self, address: String) -> &mut Self { /* ... */ }
  ```
  Sets the target IP address.

- ```rust
  pub fn with_user(self: &mut Self, user: String) -> &mut Self { /* ... */ }
  ```
  Sets the username for authentication.

- ```rust
  pub fn with_password(self: &mut Self, password: String) -> &mut Self { /* ... */ }
  ```
  Sets the password for authentication.

- ```rust
  pub fn with_sslport(self: &mut Self, sslport: u16) -> &mut Self { /* ... */ }
  ```
  Sets the SSL port for the TCP connection.

- ```rust
  pub fn build(self: &Self) -> ConnectionString { /* ... */ }
  ```
  Builds the [`ConnectionString`].

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
## Module `metadata_builder`

```rust
pub mod metadata_builder { /* ... */ }
```

### Types

#### Enum `ReferenceType`

Semantic reference types used to link a node to related type addresses.

Each variant maps to a well-known string recognized by the ctrlX Data Layer.

```rust
pub enum ReferenceType {
    Read,
    ReadIn,
    ReadOut,
    Write,
    WriteIn,
    WriteOut,
    Create,
    Uses,
    HasSave,
}
```

##### Variants

###### `Read`

###### `ReadIn`

###### `ReadOut`

###### `Write`

###### `WriteIn`

###### `WriteOut`

###### `Create`

###### `Uses`

###### `HasSave`

##### Implementations

###### Methods

- ```rust
  pub fn as_str(self: &Self) -> &''static str { /* ... */ }
  ```
  Returns the Data Layer reference type string for this variant.

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
#### Struct `MetadataBuilder`

Builder for creating ctrlX Data Layer node metadata serialized as FlatBuffers.

Use [`MetadataBuilder::new`] to start building, chain the `with_*` / `add_*`
methods to configure the metadata, and call [`MetadataBuilder::build`] to
obtain a [`crate::Value`] containing the serialized FlatBuffers payload.
```ignore
let metadata = MetadataBuilder::new("desc".to_string(), "http://example.com".to_string())
    .with_unit("°C".to_string())
    .with_display_name("Temperature".to_string())
    .with_node_class(NodeClass::Variable)
    .with_display_format(DisplayFormat::Hex)
    .with_operations_mask(OPERATION_READ | OPERATION_WRITE)
    .add_extension("extKey".to_string(), "extValue".to_string())
    .add_reference(ReferenceType::Read, "types/datalayer/read".to_string())
    .add_description("en".to_string(), "English description".to_string())
    .add_description("de".to_string(), "Deutsche Beschreibung".to_string())
    .add_display_name("en".to_string(), "Temperature".to_string())
    .add_display_name("de".to_string(), "Temperatur".to_string())
    .build();
```

```rust
pub struct MetadataBuilder {
    // Some fields omitted
}
```

##### Fields

| Name | Type | Documentation |
|------|------|---------------|
| *private fields* | ... | *Some fields have been omitted* |

##### Implementations

###### Methods

- ```rust
  pub fn new(description: String, description_url: String) -> Self { /* ... */ }
  ```
  Creates a new `MetadataBuilder` with the given human-readable description

- ```rust
  pub fn with_all_operations(self: Self) -> Self { /* ... */ }
  ```
  Enables all operations (read, write, create, delete, browse) on this node.

- ```rust
  pub fn with_operations_mask(self: Self, mask: u32) -> Self { /* ... */ }
  ```
  Set allowed operations from a bitmask.

- ```rust
  pub fn with_operations(self: Self, read: bool, write: bool, create: bool, delete: bool, browse: bool) -> Self { /* ... */ }
  ```
  Sets each operation individually.

- ```rust
  pub fn with_unit(self: Self, unit: String) -> Self { /* ... */ }
  ```
  Sets the physical unit of the node value (e.g. `"°C"`, `"rpm"`).

- ```rust
  pub fn with_display_name(self: Self, display_name: String) -> Self { /* ... */ }
  ```
  Sets a single-language display name for the node.

- ```rust
  pub fn with_node_class(self: Self, node_class: NodeClass) -> Self { /* ... */ }
  ```
  Sets the node class (e.g. `NodeClass::Variable`, `NodeClass::Folder`).

- ```rust
  pub fn with_display_format(self: Self, display_format: DisplayFormat) -> Self { /* ... */ }
  ```
  Sets the preferred display format for numeric values (e.g. `DisplayFormat::Hex`).

- ```rust
  pub fn add_extension(self: Self, key: String, value: String) -> Self { /* ... */ }
  ```
  Adds a key/value extension to the metadata.

- ```rust
  pub fn add_reference(self: Self, reference_type: ReferenceType, target_address: String) -> Self { /* ... */ }
  ```
  Adds a typed reference pointing to another Data Layer address.

- ```rust
  pub fn add_description(self: Self, id: String, text: String) -> Self { /* ... */ }
  ```
  Adds a localized description string identified by a locale ID (e.g. `"en"`, `"de"`).

- ```rust
  pub fn add_display_name(self: Self, id: String, text: String) -> Self { /* ... */ }
  ```
  Adds a localized display name identified by a locale ID (e.g. `"en"`, `"de"`).

- ```rust
  pub fn build(self: &Self) -> crate::Value { /* ... */ }
  ```
  Serializes the metadata into a [`crate::Value`] containing a FlatBuffers payload.

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
### Constants and Statics

#### Constant `OPERATION_READ`

Bitmask constants for [`MetadataBuilder::with_operations_mask`].

```rust
pub const OPERATION_READ: u32 = 0x01;
```

#### Constant `OPERATION_WRITE`

```rust
pub const OPERATION_WRITE: u32 = 0x02;
```

#### Constant `OPERATION_CREATE`

```rust
pub const OPERATION_CREATE: u32 = 0x04;
```

#### Constant `OPERATION_DELETE`

```rust
pub const OPERATION_DELETE: u32 = 0x08;
```

#### Constant `OPERATION_BROWSE`

```rust
pub const OPERATION_BROWSE: u32 = 0x10;
```

## Module `subscription_properties_builder`

```rust
pub mod subscription_properties_builder { /* ... */ }
```

### Types

#### Struct `SubscriptionPropertiesBuilder`

Builder for constructing a [`crate::Value`] containing a FlatBuffers-encoded
`SubscriptionProperties` ruleset, as required by
[`Client::create_subscription`](crate::client::Client::create_subscription).

# Example
```no_run
use ctrlx_datalayer::subscription_properties_builder::SubscriptionPropertiesBuilder;

let ruleset = SubscriptionPropertiesBuilder::new("my-sub".to_string())
    .with_publish_interval(500)
    .with_keepalive_interval(30000)
    .with_error_interval(5000)
    .with_sampling(1000)
    .build();
```

```rust
pub struct SubscriptionPropertiesBuilder {
    // Some fields omitted
}
```

##### Fields

| Name | Type | Documentation |
|------|------|---------------|
| *private fields* | ... | *Some fields have been omitted* |

##### Implementations

###### Methods

- ```rust
  pub fn new(id: String) -> Self { /* ... */ }
  ```
  Creates a new builder with the mandatory subscription `id`.

- ```rust
  pub fn with_publish_interval(self: Self, ms: u32) -> Self { /* ... */ }
  ```
  Sets the upper limit for publish messages in milliseconds.

- ```rust
  pub fn with_keepalive_interval(self: Self, ms: u32) -> Self { /* ... */ }
  ```
  Sets the keepalive interval in milliseconds used to detect provider liveness.

- ```rust
  pub fn with_error_interval(self: Self, ms: u32) -> Self { /* ... */ }
  ```
  Sets the re-read interval in milliseconds for nodes that returned an error.

- ```rust
  pub fn with_sampling(self: Self, sampling_interval_us: u64) -> Self { /* ... */ }
  ```
  Adds a **Sampling** rule.

- ```rust
  pub fn with_queueing(self: Self, queue_size: u32, behaviour: QueueBehaviour) -> Self { /* ... */ }
  ```
  Adds a **Queueing** rule.

- ```rust
  pub fn with_data_change_filter(self: Self, dead_band_value: f32) -> Self { /* ... */ }
  ```
  Adds a **DataChangeFilter** rule.

- ```rust
  pub fn with_change_events(self: Self, value_change: DataChangeTrigger, browselist_change: bool, metadata_change: bool, connection_change: bool) -> Self { /* ... */ }
  ```
  Adds a **ChangeEvents** rule to subscribe to event-based updates.

- ```rust
  pub fn with_counting(self: Self, count_subscriptions: bool) -> Self { /* ... */ }
  ```
  Adds a **Counting** rule.

- ```rust
  pub fn with_lossless_rate_limit(self: Self, rate_limit: u32) -> Self { /* ... */ }
  ```
  Adds a **LosslessRateLimit** rule.

- ```rust
  pub fn with_lossless_interval(self: Self, sampling_interval_us: u64, tolerance_us: u64) -> Self { /* ... */ }
  ```
  Adds a **LosslessInterval** rule.

- ```rust
  pub fn build(self: &Self) -> crate::Value { /* ... */ }
  ```
  Serializes the subscription properties into a FlatBuffers-encoded [`crate::Value`].

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **RefUnwindSafe**
- **Send**
- **Sync**
- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
## Module `schema`

Built-in schema identifiers for the ctrlX Data Layer.

These values correspond to the `DLR_SCHEMA_DLR_SCHEMA_*` constants defined
in the C API (`comm/datalayer/c/comm_datalayer.h`).

```rust
pub mod schema { /* ... */ }
```

### Types

#### Enum `Schema`

Identifies one of the built-in FlatBuffers schemas provided by the datalayer runtime.

```rust
pub enum Schema {
    Metadata,
    Reflection,
    Memory,
    MemoryMap,
    Token,
    Problem,
    Diagnosis,
    UnknownCode(std::os::raw::c_uint),
}
```

##### Variants

###### `Metadata`

Schema for node metadata descriptions.

###### `Reflection`

Schema for FlatBuffers reflection (binary schema).

###### `Memory`

Schema for shared-memory definitions.

###### `MemoryMap`

Schema for shared-memory map definitions.

###### `Token`

Schema for security tokens.

###### `Problem`

Schema for RFC 7807 problem details.

###### `Diagnosis`

Schema for diagnosis information.

###### `UnknownCode`

Returned when the C library reports an unrecognised schema code.

Fields:

| Index | Type | Documentation |
|-------|------|---------------|
| 0 | `std::os::raw::c_uint` |  |

##### Implementations

###### Trait Implementations

- **Any**
  - ```rust
    fn type_id(self: &Self) -> TypeId { /* ... */ }
    ```

- **Borrow**
  - ```rust
    fn borrow(self: &Self) -> &T { /* ... */ }
    ```

- **BorrowMut**
  - ```rust
    fn borrow_mut(self: &mut Self) -> &mut T { /* ... */ }
    ```

- **Clone**
  - ```rust
    fn clone(self: &Self) -> Schema { /* ... */ }
    ```

- **CloneToUninit**
  - ```rust
    unsafe fn clone_to_uninit(self: &Self, dest: *mut u8) { /* ... */ }
    ```

- **Copy**
- **Debug**
  - ```rust
    fn fmt(self: &Self, f: &mut $crate::fmt::Formatter<''_>) -> $crate::fmt::Result { /* ... */ }
    ```

- **Display**
  - ```rust
    fn fmt(self: &Self, f: &mut fmt::Formatter<''_>) -> fmt::Result { /* ... */ }
    ```

- **Eq**
- **Freeze**
- **From**
  - ```rust
    fn from(t: T) -> T { /* ... */ }
    ```
    Returns the argument unchanged.

  - ```rust
    fn from(code: std::os::raw::c_uint) -> Self { /* ... */ }
    ```

  - ```rust
    fn from(schema: Schema) -> Self { /* ... */ }
    ```

- **Into**
  - ```rust
    fn into(self: Self) -> U { /* ... */ }
    ```
    Calls `U::from(self)`.

- **PartialEq**
  - ```rust
    fn eq(self: &Self, other: &Schema) -> bool { /* ... */ }
    ```

- **RefUnwindSafe**
- **Send**
- **StructuralPartialEq**
- **Sync**
- **ToOwned**
  - ```rust
    fn to_owned(self: &Self) -> T { /* ... */ }
    ```

  - ```rust
    fn clone_into(self: &Self, target: &mut T) { /* ... */ }
    ```

- **ToString**
  - ```rust
    fn to_string(self: &Self) -> String { /* ... */ }
    ```

- **TryFrom**
  - ```rust
    fn try_from(value: U) -> Result<T, <T as TryFrom<U>>::Error> { /* ... */ }
    ```

- **TryInto**
  - ```rust
    fn try_into(self: Self) -> Result<U, <U as TryFrom<T>>::Error> { /* ... */ }
    ```

- **Unpin**
- **UnsafeUnpin**
- **UnwindSafe**
## Re-exports

### Re-export `Result`

```rust
pub use crate::result::Result;
```

### Re-export `System`

```rust
pub use crate::system::System;
```

### Re-export `Factory`

```rust
pub use crate::factory::Factory;
```

### Re-export `Client`

```rust
pub use crate::client::Client;
```

### Re-export `Provider`

```rust
pub use crate::provider::Provider;
```

### Re-export `ProviderNode`

```rust
pub use crate::provider::node::ProviderNode;
```

### Re-export `NotifyItemPublish`

```rust
pub use crate::provider::subscription::NotifyItemPublish;
```

### Re-export `Converter`

```rust
pub use crate::converter::Converter;
```

### Re-export `TimeoutSetting`

```rust
pub use crate::timeout_settings::TimeoutSetting;
```

### Re-export `ConnectionBuilder`

```rust
pub use crate::connection_builder::ConnectionBuilder;
```

### Re-export `MetadataBuilder`

```rust
pub use crate::metadata_builder::MetadataBuilder;
```

### Re-export `SubscriptionPropertiesBuilder`

```rust
pub use crate::subscription_properties_builder::SubscriptionPropertiesBuilder;
```

### Re-export `VariantType`

```rust
pub use crate::variant::VariantType;
```

### Re-export `Value`

```rust
pub use crate::variant::Value;
```

### Re-export `Schema`

```rust
pub use crate::schema::Schema;
```

### Re-export `Subscription`

```rust
pub use crate::client::subscription::Subscription;
```

### Re-export `SubscriptionCallback`

```rust
pub use crate::client::subscription::SubscriptionCallback;
```

### Re-export `SubscriptionCallbackBox`

```rust
pub use crate::client::subscription::SubscriptionCallbackBox;
```

### Re-export `SyncBulk`

```rust
pub use crate::client::bulk::sync_bulk::SyncBulk;
```

### Re-export `AsyncBulk`

```rust
pub use crate::client::bulk::async_bulk::AsyncBulk;
```

### Re-export `ResponseCallback`

```rust
pub use crate::client::bulk::ResponseCallback;
```

### Re-export `BulkReadArg`

```rust
pub use crate::client::bulk::BulkReadArg;
```

### Re-export `BulkWriteArg`

```rust
pub use crate::client::bulk::BulkWriteArg;
```

### Re-export `BulkCreateArg`

```rust
pub use crate::client::bulk::BulkCreateArg;
```

### Re-export `BulkResponse`

```rust
pub use crate::client::bulk::BulkResponse;
```

