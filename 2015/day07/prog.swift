import Foundation

protocol SignalSource {
    var signal: UInt16? { get }
}

class Wire: SignalSource, CustomDebugStringConvertible {
    let identifier: String
    var signalSource: SignalSource? {
        didSet {
            cachedSignal = nil
        }
    }

    func reset() {
        cachedSignal = nil
    }

    init(identifier: String) {
        self.identifier = identifier
    }

    private var cachedSignal: UInt16?

    var signal: UInt16? {
        if let cachedSignal = cachedSignal {
            return cachedSignal
        }

        cachedSignal = signalSource?.signal
        return cachedSignal
    }

    var debugDescription: String {
        return identifier
    }
}

var wires: [String: Wire] = [:]

struct ConstantSignal: SignalSource, CustomDebugStringConvertible {
    let constant: UInt16

    var signal: UInt16? {
        return constant
    }

    var debugDescription: String {
        return String(constant)
    }
}

enum Gate: SignalSource {
    case Not(SignalSource)
    case And(SignalSource, SignalSource)
    case Or(SignalSource, SignalSource)
    case LShift(SignalSource, SignalSource)
    case RShift(SignalSource, SignalSource)

    var signal: UInt16? {
        switch self {
        case let .Not(source):
            guard let value = source.signal else { return nil }

            return ~value
        case let .And(source1, source2):
            guard let value1 = source1.signal,
                value2 = source2.signal else { return nil }

            return value1 & value2
        case let .Or(source1, source2):
            guard let value1 = source1.signal,
                value2 = source2.signal else { return nil }

            return value1 | value2
        case let .LShift(source1, source2):
            guard let value1 = source1.signal,
                value2 = source2.signal else { return nil }

            return value1 << value2
        case let .RShift(source1, source2):
            guard let value1 = source1.signal,
                value2 = source2.signal else { return nil }

            return value1 >> value2
        }
    }
}

let DigitCharacterSet = NSCharacterSet(charactersInString: "1234567890")

extension String {
    var isOnlyDigits: Bool {
        let representation = self as NSString

        for i in 0 ..< representation.length {
            let uc = representation.characterAtIndex(i)

            if !DigitCharacterSet.characterIsMember(uc) {
                return false
            }
        }

        return true
    }
}

func WireForIdentifier(identifier: String) -> Wire {
    if let wire = wires[identifier] {
        return wire
    }
    else {
        let wire = Wire(identifier: identifier)
        wires[identifier] = wire
        return wire
    }
}

func SignalSourceForString(string: String) -> SignalSource? {
    if string.isOnlyDigits {
        guard let constant = UInt16(string) else { return nil }
        return ConstantSignal(constant: constant)
    }

    return WireForIdentifier(string)
}

func ParseCommandString(string: String) {
    let components = string
        .characters
        .split(" ")
        .flatMap(String.init)

    guard components.count >= 3 else { return }

    if components.count == 3 && components[1] == "->" {
        guard let source = SignalSourceForString(components[0])
            else { return }

        let destination = WireForIdentifier(components[2])

        destination.signalSource = source
    }
    else if components[0] == "NOT" &&
        components.count == 4 &&
        components[2] == "->" {
            guard let source = SignalSourceForString(components[1])
                else { return }

            let destination = WireForIdentifier(components[3])

            let gate = Gate.Not(source)

            destination.signalSource = gate
    }
    else if components[1] == "AND" &&
        components.count == 5 &&
        components[3] == "->" {
            guard let
                source1 = SignalSourceForString(components[0]),
                source2 = SignalSourceForString(components[2])
                else { return }

            let destination = WireForIdentifier(components[4])

            let gate = Gate.And(source1, source2)

            destination.signalSource = gate
    }
    else if components[1] == "OR" &&
        components.count == 5 &&
        components[3] == "->" {
            guard let
                source1 = SignalSourceForString(components[0]),
                source2 = SignalSourceForString(components[2])
                else { return }

            let destination = WireForIdentifier(components[4])

            let gate = Gate.Or(source1, source2)

            destination.signalSource = gate
    }
    else if components[1] == "LSHIFT" &&
        components.count == 5 &&
        components[3] == "->" {
            guard let
                source1 = SignalSourceForString(components[0]),
                source2 = SignalSourceForString(components[2])
                else { return }

            let destination = WireForIdentifier(components[4])

            let gate = Gate.LShift(source1, source2)

            destination.signalSource = gate
    }
    else if components[1] == "RSHIFT" &&
        components.count == 5 &&
        components[3] == "->" {
            guard let
                source1 = SignalSourceForString(components[0]),
                source2 = SignalSourceForString(components[2])
                else { return }


            let destination = WireForIdentifier(components[4])

            let gate = Gate.RShift(source1, source2)

            destination.signalSource = gate
    }
    else {
        print(string)
    }
}

func ParseCommandStrings(strings: [String]) {
    for string in strings {
        ParseCommandString(string)
    }
}

let input = "af AND ah -> ai\nNOT lk -> ll\nhz RSHIFT 1 -> is\nNOT go -> gp\ndu OR dt -> dv\nx RSHIFT 5 -> aa\nat OR az -> ba\neo LSHIFT 15 -> es\nci OR ct -> cu\nb RSHIFT 5 -> f\nfm OR fn -> fo\nNOT ag -> ah\nv OR w -> x\ng AND i -> j\nan LSHIFT 15 -> ar\n1 AND cx -> cy\njq AND jw -> jy\niu RSHIFT 5 -> ix\ngl AND gm -> go\nNOT bw -> bx\njp RSHIFT 3 -> jr\nhg AND hh -> hj\nbv AND bx -> by\ner OR es -> et\nkl OR kr -> ks\net RSHIFT 1 -> fm\ne AND f -> h\nu LSHIFT 1 -> ao\nhe RSHIFT 1 -> hx\neg AND ei -> ej\nbo AND bu -> bw\ndz OR ef -> eg\ndy RSHIFT 3 -> ea\ngl OR gm -> gn\nda LSHIFT 1 -> du\nau OR av -> aw\ngj OR gu -> gv\neu OR fa -> fb\nlg OR lm -> ln\ne OR f -> g\nNOT dm -> dn\nNOT l -> m\naq OR ar -> as\ngj RSHIFT 5 -> gm\nhm AND ho -> hp\nge LSHIFT 15 -> gi\njp RSHIFT 1 -> ki\nhg OR hh -> hi\nlc LSHIFT 1 -> lw\nkm OR kn -> ko\neq LSHIFT 1 -> fk\n1 AND am -> an\ngj RSHIFT 1 -> hc\naj AND al -> am\ngj AND gu -> gw\nko AND kq -> kr\nha OR gz -> hb\nbn OR by -> bz\niv OR jb -> jc\nNOT ac -> ad\nbo OR bu -> bv\nd AND j -> l\nbk LSHIFT 1 -> ce\nde OR dk -> dl\ndd RSHIFT 1 -> dw\nhz AND ik -> im\nNOT jd -> je\nfo RSHIFT 2 -> fp\nhb LSHIFT 1 -> hv\nlf RSHIFT 2 -> lg\ngj RSHIFT 3 -> gl\nki OR kj -> kk\nNOT ak -> al\nld OR le -> lf\nci RSHIFT 3 -> ck\n1 AND cc -> cd\nNOT kx -> ky\nfp OR fv -> fw\nev AND ew -> ey\ndt LSHIFT 15 -> dx\nNOT ax -> ay\nbp AND bq -> bs\nNOT ii -> ij\nci AND ct -> cv\niq OR ip -> ir\nx RSHIFT 2 -> y\nfq OR fr -> fs\nbn RSHIFT 5 -> bq\n0 -> c\n14146 -> b\nd OR j -> k\nz OR aa -> ab\ngf OR ge -> gg\ndf OR dg -> dh\nNOT hj -> hk\nNOT di -> dj\nfj LSHIFT 15 -> fn\nlf RSHIFT 1 -> ly\nb AND n -> p\njq OR jw -> jx\ngn AND gp -> gq\nx RSHIFT 1 -> aq\nex AND ez -> fa\nNOT fc -> fd\nbj OR bi -> bk\nas RSHIFT 5 -> av\nhu LSHIFT 15 -> hy\nNOT gs -> gt\nfs AND fu -> fv\ndh AND dj -> dk\nbz AND cb -> cc\ndy RSHIFT 1 -> er\nhc OR hd -> he\nfo OR fz -> ga\nt OR s -> u\nb RSHIFT 2 -> d\nNOT jy -> jz\nhz RSHIFT 2 -> ia\nkk AND kv -> kx\nga AND gc -> gd\nfl LSHIFT 1 -> gf\nbn AND by -> ca\nNOT hr -> hs\nNOT bs -> bt\nlf RSHIFT 3 -> lh\nau AND av -> ax\n1 AND gd -> ge\njr OR js -> jt\nfw AND fy -> fz\nNOT iz -> ja\nc LSHIFT 1 -> t\ndy RSHIFT 5 -> eb\nbp OR bq -> br\nNOT h -> i\n1 AND ds -> dt\nab AND ad -> ae\nap LSHIFT 1 -> bj\nbr AND bt -> bu\nNOT ca -> cb\nNOT el -> em\ns LSHIFT 15 -> w\ngk OR gq -> gr\nff AND fh -> fi\nkf LSHIFT 15 -> kj\nfp AND fv -> fx\nlh OR li -> lj\nbn RSHIFT 3 -> bp\njp OR ka -> kb\nlw OR lv -> lx\niy AND ja -> jb\ndy OR ej -> ek\n1 AND bh -> bi\nNOT kt -> ku\nao OR an -> ap\nia AND ig -> ii\nNOT ey -> ez\nbn RSHIFT 1 -> cg\nfk OR fj -> fl\nce OR cd -> cf\neu AND fa -> fc\nkg OR kf -> kh\njr AND js -> ju\niu RSHIFT 3 -> iw\ndf AND dg -> di\ndl AND dn -> do\nla LSHIFT 15 -> le\nfo RSHIFT 1 -> gh\nNOT gw -> gx\nNOT gb -> gc\nir LSHIFT 1 -> jl\nx AND ai -> ak\nhe RSHIFT 5 -> hh\n1 AND lu -> lv\nNOT ft -> fu\ngh OR gi -> gj\nlf RSHIFT 5 -> li\nx RSHIFT 3 -> z\nb RSHIFT 3 -> e\nhe RSHIFT 2 -> hf\nNOT fx -> fy\njt AND jv -> jw\nhx OR hy -> hz\njp AND ka -> kc\nfb AND fd -> fe\nhz OR ik -> il\nci RSHIFT 1 -> db\nfo AND fz -> gb\nfq AND fr -> ft\ngj RSHIFT 2 -> gk\ncg OR ch -> ci\ncd LSHIFT 15 -> ch\njm LSHIFT 1 -> kg\nih AND ij -> ik\nfo RSHIFT 3 -> fq\nfo RSHIFT 5 -> fr\n1 AND fi -> fj\n1 AND kz -> la\niu AND jf -> jh\ncq AND cs -> ct\ndv LSHIFT 1 -> ep\nhf OR hl -> hm\nkm AND kn -> kp\nde AND dk -> dm\ndd RSHIFT 5 -> dg\nNOT lo -> lp\nNOT ju -> jv\nNOT fg -> fh\ncm AND co -> cp\nea AND eb -> ed\ndd RSHIFT 3 -> df\ngr AND gt -> gu\nep OR eo -> eq\ncj AND cp -> cr\nlf OR lq -> lr\ngg LSHIFT 1 -> ha\net RSHIFT 2 -> eu\nNOT jh -> ji\nek AND em -> en\njk LSHIFT 15 -> jo\nia OR ig -> ih\ngv AND gx -> gy\net AND fe -> fg\nlh AND li -> lk\n1 AND io -> ip\nkb AND kd -> ke\nkk RSHIFT 5 -> kn\nid AND if -> ig\nNOT ls -> lt\ndw OR dx -> dy\ndd AND do -> dq\nlf AND lq -> ls\nNOT kc -> kd\ndy AND ej -> el\n1 AND ke -> kf\net OR fe -> ff\nhz RSHIFT 5 -> ic\ndd OR do -> dp\ncj OR cp -> cq\nNOT dq -> dr\nkk RSHIFT 1 -> ld\njg AND ji -> jj\nhe OR hp -> hq\nhi AND hk -> hl\ndp AND dr -> ds\ndz AND ef -> eh\nhz RSHIFT 3 -> ib\ndb OR dc -> dd\nhw LSHIFT 1 -> iq\nhe AND hp -> hr\nNOT cr -> cs\nlg AND lm -> lo\nhv OR hu -> hw\nil AND in -> io\nNOT eh -> ei\ngz LSHIFT 15 -> hd\ngk AND gq -> gs\n1 AND en -> eo\nNOT kp -> kq\net RSHIFT 5 -> ew\nlj AND ll -> lm\nhe RSHIFT 3 -> hg\net RSHIFT 3 -> ev\nas AND bd -> bf\ncu AND cw -> cx\njx AND jz -> ka\nb OR n -> o\nbe AND bg -> bh\n1 AND ht -> hu\n1 AND gy -> gz\nNOT hn -> ho\nck OR cl -> cm\nec AND ee -> ef\nlv LSHIFT 15 -> lz\nks AND ku -> kv\nNOT ie -> if\nhf AND hl -> hn\n1 AND r -> s\nib AND ic -> ie\nhq AND hs -> ht\ny AND ae -> ag\nNOT ed -> ee\nbi LSHIFT 15 -> bm\ndy RSHIFT 2 -> dz\nci RSHIFT 2 -> cj\nNOT bf -> bg\nNOT im -> in\nev OR ew -> ex\nib OR ic -> id\nbn RSHIFT 2 -> bo\ndd RSHIFT 2 -> de\nbl OR bm -> bn\nas RSHIFT 1 -> bl\nea OR eb -> ec\nln AND lp -> lq\nkk RSHIFT 3 -> km\nis OR it -> iu\niu RSHIFT 2 -> iv\nas OR bd -> be\nip LSHIFT 15 -> it\niw OR ix -> iy\nkk RSHIFT 2 -> kl\nNOT bb -> bc\nci RSHIFT 5 -> cl\nly OR lz -> ma\nz AND aa -> ac\niu RSHIFT 1 -> jn\ncy LSHIFT 15 -> dc\ncf LSHIFT 1 -> cz\nas RSHIFT 3 -> au\ncz OR cy -> da\nkw AND ky -> kz\nlx -> a\niw AND ix -> iz\nlr AND lt -> lu\njp RSHIFT 5 -> js\naw AND ay -> az\njc AND je -> jf\nlb OR la -> lc\nNOT cn -> co\nkh LSHIFT 1 -> lb\n1 AND jj -> jk\ny OR ae -> af\nck AND cl -> cn\nkk OR kv -> kw\nNOT cv -> cw\nkl AND kr -> kt\niu OR jf -> jg\nat AND az -> bb\njp RSHIFT 2 -> jq\niv AND jb -> jd\njn OR jo -> jp\nx OR ai -> aj\nba AND bc -> bd\njl OR jk -> jm\nb RSHIFT 1 -> v\no AND q -> r\nNOT p -> q\nk AND m -> n\nas RSHIFT 2 -> at\n"

let exampleCommands = ["123 -> x",
    "456 -> y",
    "x AND y -> d",
    "x OR y -> e",
    "x LSHIFT 2 -> f",
    "y RSHIFT 2 -> g",
    "NOT x -> h",
    "NOT y -> i"]

// Example:
ParseCommandString("123 -> x")
ParseCommandString("456 -> y")
ParseCommandString("x AND y -> d")
ParseCommandString("x OR y -> e")
ParseCommandString("x LSHIFT 2 -> f")
ParseCommandString("y RSHIFT 2 -> g")
ParseCommandString("NOT x -> h")
ParseCommandString("NOT y -> i")

let result = wires
    .sort { $0.0.compare($1.0) == .OrderedAscending }
    .filter { $1.signal != nil }
    .map { "\($0): \($1.signal!)" }
    .joinWithSeparator("\n")

result

wires.removeAll()

let commands = input.characters.split("\n").flatMap(String.init)
commands.count

ParseCommandStrings(commands)

wires["a"]?.signal

ParseCommandString("956 -> b")

wires["b"]?.signalSource

for (_, wire) in wires {
    wire.reset()
}

wires["a"]?.signal
