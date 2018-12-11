type t =
  (
    ~key: string=?,
    ~ref: React.Ref.t(Dom.element)=?,
    ~props: ReactDomProps.t=?,
    array(React.Element.t)
  ) =>
  React.Element.t;

let create:
  (
    string,
    ~key: string=?,
    ~ref: React.Ref.t(Dom.element)=?,
    ~props: ReactDomProps.t=?,
    array(React.Element.t)
  ) =>
  React.Element.t;