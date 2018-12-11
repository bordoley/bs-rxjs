type component('props, 'children);
type element;

module Element = {
  type t = element;

  let makeReactProps =
      (key: option(string), props: 'props, children: 'children): Js.t({..}) => {
    "key": key,
    "reasonProps": props,
    "reasonChildren": children,
  };

  [@bs.val] [@bs.module "react"]
  external reactCreateElement:
    (component('props, 'children), Js.t({..})) => t =
    "createElement";

  [@bs.val] [@bs.module "react"]
  external reactCreateElementWithChildren:
    (component('props, 'children), ~props: Js.t({..}), array(t)) => t =
    "createElement";

  [@bs.val] [@bs.module "react"]
  external reactCreateElementWithChildrenVariadic: 'a = "createElement";

  let create =
      (
        component: component('props, 'children),
        ~key: option(string)=?,
        ~props: 'props,
        children,
      )
      : t =>
    reactCreateElement(component, makeReactProps(key, props, children));

  external array: array(t) => t = "%identity";
  [@bs.val] external null: t = "null";
};

let referenceEquality = (a, b) => a === b;

module Component = {
  type t('props, 'children) = component('props, 'children);
  [@bs.val]
  external defineProperty: ('a, string, Js.t({..})) => unit =
    "Object.defineProperty";

  external reactCreateComponent:
    (Js.t({..}) => element) => t('props, 'children) =
    "%identity";

  let createComponent =
      (
        ~name: option(string)=?,
        f: (~props: 'props, ~children: 'children) => element,
      )
      : t('props, 'children) => {
    let component = reactProps => {
      let props = reactProps##reasonProps;
      let children = reactProps##reasonChildren;
      f(~props, ~children);
    };

    switch (name) {
    | Some(name) =>
      defineProperty(component, "name", {"value": name, "writable": false})
    | _ => ()
    };

    reactCreateComponent(component);
  };

  [@bs.val] [@bs.module "react"]
  external reactMemo:
    (t('props, 'children), (Js.t({..}), Js.t({..})) => bool) =>
    t('props, 'children) =
    "memo";

  let reasonPropsAndChildrenAreReferenceEqual =
      (a: Js.t({..}), b: Js.t({..})) =>
    a##reasonProps ===
    b##reasonProps
    &&
    a##reasonChildren ===
    b##reasonChildren;

  let create =
      (
        ~name: option(string)=?,
        ~arePropsEqual: ('props, 'props) => bool=referenceEquality,
        ~areChildrenEqual: ('children, 'children) => bool=referenceEquality,
        f: (~props: 'props, ~children: 'children) => element,
      )
      : t('props, 'children) => {
    let areEqual =
      arePropsEqual === referenceEquality
      && areChildrenEqual === referenceEquality ?
        reasonPropsAndChildrenAreReferenceEqual :
        (
          (a: Js.t({..}), b: Js.t({..})) =>
            arePropsEqual(
              Obj.magic(a##reasonProps),
              Obj.magic(b##reasonProps),
            )
            && areChildrenEqual(
                 Obj.magic(a##reasonChildren),
                 Obj.magic(b##reasonChildren),
               )
        );
    let component = createComponent(~name?, f);
    reactMemo(component, areEqual);
  };
};

module Ref = {
  [@bs.deriving abstract]
  type t('a) = {current: option('a)};
};

module JsComponent = {
  type t('props);

  external cast: 'a => t('props) = "%identity";
  external toComponent: t('props) => Component.t('props, array(Element.t)) = "%identity";
};

let createComponent =
    (
      ~name: option(string)=?,
      ~arePropsEqual: ('props, 'props) => bool=referenceEquality,
      ~areChildrenEqual: ('children, 'children) => bool=referenceEquality,
      f: (~props: 'props, ~children: 'children) => element,
    ) => {
  let component =
    Component.create(~name?, ~arePropsEqual, ~areChildrenEqual, f);
  Element.create(component);
};

let createComponentWithDefaultProps =
    (
      ~name: option(string)=?,
      ~arePropsEqual: ('props, 'props) => bool=referenceEquality,
      ~areChildrenEqual: ('children, 'children) => bool=referenceEquality,
      ~defaultProps: 'props,
      f: (~props: 'props, ~children: 'children) => element,
    ) => {
  let reasonComponent =
    createComponent(~name?, ~arePropsEqual, ~areChildrenEqual, f);
  (~key=?, ~props=defaultProps, children) =>
    reasonComponent(~key?, ~props, children);
};

let makeReactProps = (key: option(string), ref, props: 'props) => {
  let keyObj = {"key": key, "ref": ref};
  Js.Obj.assign(Js.Obj.assign(Js.Obj.empty(), Obj.magic(props)), keyObj);
};

let wrapJsComponent = (jsComponent, ~key=?, ~ref=?, ~props, children): Element.t => {
  let childrenLength = Js.Array.length(children);
  let jsProps = makeReactProps(key, ref, props);

  let component = jsComponent |> JsComponent.toComponent;

  switch (childrenLength) {
  | _ when childrenLength <= 10 =>
    /* Suppress missing key warnings in the common case. */
    let vararg =
      [|Obj.magic(component), Obj.magic(jsProps)|]
      |> Js.Array.concat(children);
    Obj.magic(Element.reactCreateElementWithChildrenVariadic)##apply(
      Js.Nullable.null,
      vararg,
    );
  | _ =>
    Element.reactCreateElementWithChildren(
      component,
      ~props=jsProps,
      children,
    )
  };
};

let wrapJsComponentWithDefaultProps =
    (component, ~defaultProps, ~key=?, ~ref=?, ~props=defaultProps, children) =>
  wrapJsComponent(component, ~key?, ~ref?, ~props, children);

module Context = {
  type t('a);

  [@bs.val] [@bs.module "react"]
  external create: 'a => t('a) = "createContext";
};

[@bs.val] [@bs.module "react"]
external useContext: Context.t('a) => 'a = "useContext";

[@bs.val] [@bs.module "react"]
external reactUseEffect: (unit => Js.null(unit)) => unit = "useEffect";
let useEffect = generator =>
  reactUseEffect(() => {
    generator();
    Js.Null.empty;
  });

[@bs.val] [@bs.module "react"]
external reactUseEffect1: (unit => Js.null(unit), array('key)) => unit =
  "useEffect";
let useEffect1 = (generator, key) =>
  reactUseEffect1(
    () => {
      generator(key);
      Js.Null.empty;
    },
    [|key|],
  );

[@bs.val] [@bs.module "react"]
external reactUseEffect2: (unit => Js.null(unit), ('k0, 'k1)) => unit =
  "useEffect";
let useEffect2 = (generator, k0, k1) =>
  reactUseEffect2(
    () => {
      generator(k0, k1);
      Js.Null.empty;
    },
    (k0, k1),
  );

[@bs.val] [@bs.module "react"]
external reactUseEffect3: (unit => Js.null(unit), ('k0, 'k1, 'k2)) => unit =
  "useEffect";
let useEffect3 = (generator, k0, k1, k2) =>
  reactUseEffect3(
    () => {
      generator(k0, k1, k2);
      Js.Null.empty;
    },
    (k0, k1, k2),
  );

type cleanup = unit => unit;

[@bs.val] [@bs.module "react"]
external reactUseEffectWithCleanup: (unit => cleanup) => unit = "useEffect";
let useEffectWithCleanup = generator => reactUseEffectWithCleanup(generator);

[@bs.val] [@bs.module "react"]
external reactUseEffectWithCleanup1: (unit => cleanup, array('key)) => unit =
  "useEffect";
let useEffectWithCleanup1 = (generator, key) =>
  reactUseEffectWithCleanup1(() => generator(key), [|key|]);

[@bs.val] [@bs.module "react"]
external reactUseEffectWithCleanup2: (unit => cleanup, ('k0, 'k1)) => unit =
  "useEffect";
let useEffectWithCleanup2 = (generator, k0, k1) =>
  reactUseEffectWithCleanup2(() => generator(k0, k1), (k0, k1));

[@bs.val] [@bs.module "react"]
external reactUseEffectWithCleanup3:
  (unit => cleanup, ('k0, 'k1, 'k2)) => unit =
  "useEffect";
let useEffectWithCleanup3 = (generator, k0, k1, k2) =>
  reactUseEffectWithCleanup3(() => generator(k0, k1, k2), (k0, k1, k2));

[@bs.val] [@bs.module "react"]
external reactUseEffectWithCleanup4:
  (unit => cleanup, ('k0, 'k1, 'k2, 'k3)) => unit =
  "useEffect";
let useEffectWithCleanup4 = (generator, k0, k1, k2, k3) =>
  reactUseEffectWithCleanup4(
    () => generator(k0, k1, k2, k3),
    (k0, k1, k2, k3),
  );

[@bs.val] [@bs.module "react"]
external reactUseMemo: (unit => 't) => 't = "useMemo";
let useMemo = (generator: unit => 't) => reactUseMemo(generator);

[@bs.val] [@bs.module "react"]
external reactUseMemo1: (unit => 't, array('a)) => 't = "useMemo";
let useMemo1 = (generator: 'a => 't, ctx0: 'a) =>
  reactUseMemo1(() => generator(ctx0), [|ctx0|]);

[@bs.val] [@bs.module "react"]
external reactUseMemo2: (unit => 't, ('a, 'b)) => 't = "useMemo";
let useMemo2 = (generator: ('a, 'b) => 't, ctx0: 'a, ctx1: 'b) =>
  reactUseMemo2(() => generator(ctx0, ctx1), (ctx0, ctx1));

[@bs.val] [@bs.module "react"]
external reactUseMemo3: (unit => 't, ('a, 'b, 'c)) => 't = "useMemo";
let useMemo3 = (generator: ('a, 'b, 'c) => 't, ctx0: 'a, ctx1: 'b, ctx2: 'c) =>
  reactUseMemo3(() => generator(ctx0, ctx1, ctx2), (ctx0, ctx1, ctx2));

[@bs.val] [@bs.module "react"]
external reactUseMemo4: (unit => 't, ('a, 'b, 'c, 'd)) => 't = "useMemo";
let useMemo4 =
    (
      generator: ('a, 'b, 'c, 'd) => 't,
      ctx0: 'a,
      ctx1: 'b,
      ctx2: 'c,
      ctx3: 'd,
    ) =>
  reactUseMemo4(
    () => generator(ctx0, ctx1, ctx2, ctx3),
    (ctx0, ctx1, ctx2, ctx3),
  );

[@bs.val] [@bs.module "react"]
external useRef: option('a) => Ref.t('a) = "useRef";

[@bs.val] [@bs.module "react"]
external reactUseState: 'state => ('state, 'state => unit) = "useState";
let useState = reactUseState;

let null = (~key as _=?, ~props as _, _) => Element.null;

let raise = (~key as _=?, ~props as exn, _): Element.t => raise(exn);