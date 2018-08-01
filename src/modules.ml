open Json
open Types

module type WithType = sig
  type t
end

module type WithDecoder = sig
  include WithType

  val decoder : t Decode.decoder 
end

module type WithEncoder = sig
  include WithType
  val encoder : t encoder
end
