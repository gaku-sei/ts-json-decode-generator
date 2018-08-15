module type NODE = {include (module type of Node);};

include (Node: NODE with module Fs := Node.Fs and module Path := Node.Path);

module Fs = {
  include Node.Fs;

  [@bs.module "fs"] external mkdir: string => unit = "mkdirSync";
};

module Path = {
  include Node.Path;

  [@bs.module "path"] external separator: string = "sep";
};