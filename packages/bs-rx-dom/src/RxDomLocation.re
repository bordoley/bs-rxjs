type history;
[@bs.val] [@bs.scope "window"] external history: history = "history";
[@bs.send.pipe: history]
external pushState: (option(unit), string, string) => unit = "";

type location;
[@bs.val] [@bs.scope "window"] external location: location = "location";
[@bs.get] external href: location => string = "";

[@bs.val] external window: Dom.eventTarget = "window";

type t = unit;

let instance: t = ();

let observable =
  window
  |> RxObservable.fromEvent(~passive=true, "popstate")
  |> RxObservables.map(_ => location |> href)
  |> RxObservables.startWith(location |> href)
  |> RxObservables.shareReplayLast;

let asObservable = _ => observable;

let update = (f, _) => {
  let value = location |> href;
  let nextValue = f(value);
  if (value !== nextValue) {
     history |> pushState(None, "", nextValue);
   };
};