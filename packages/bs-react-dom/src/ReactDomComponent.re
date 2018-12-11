type t =
  (
    ~key: string=?,
    ~ref: React.Ref.t(Dom.element)=?,
    ~props: ReactDomProps.t=?,
    array(React.Element.t)
  ) =>
  React.Element.t;

let create = tag =>
  React.wrapJsComponentWithDefaultProps(
    React.JsComponent.cast(tag),
    ~defaultProps=ReactDomProps.default,
  );