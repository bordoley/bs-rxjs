type t;

include RxObservable.S with type t := t and type a := string;

let instance: t;

let update: (string => string, t) => unit;