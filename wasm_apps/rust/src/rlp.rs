use std::collections::HashMap;

// Legacy Transaction Fields
const TRANSACTION_FIELDS: &[(&str, usize, bool)] = &[
    ("nonce", 32, true),
    ("gasPrice", 32, true),
    ("gasLimit", 32, true),
    ("to", 20, false),
    ("value", 32, true),
    ("data", 0, false),
];

fn _serialize(transaction: &HashMap<&str, &[u8]>, signature: Option<&[u8]>) -> Vec<u8> {
    let mut raw: Vec<&[u8]> = Vec::new();

    for field_info in TRANSACTION_FIELDS {
        let field_name = field_info.0;
        let max_length = field_info.1;
        let is_numeric = field_info.2;

        let mut value = match transaction.get(field_name) {
            Some(v) => v,
            None => &[][..],
        };

        if is_numeric {
            // TODO: add padding here
        }

        if max_length > 0 && value.len() != max_length && value.len() > 0 {
            // TODO: throw argument error here
        }

        if max_length > 0 {
            value = &strip_zeros(value);
            if value.len() > max_length {
                // TODO: throw argument error here
            }
        }

        raw.push(value);
    }

    let mut chain_id = 0;
    if let Some(cid) = transaction.get("chainId") {
        chain_id = cid;
        if !chain_id.is_number() {
            // TODO: throw argument error here
        }
    } else if let Some(sig) = signature {
        if !sig.is_bytes_like() && sig.v > 28 {
            chain_id = (sig.v - 35) / 2;
        }
    }

    if chain_id != 0 {
        raw.push(&chain_id.to_le_bytes());
        raw.push(&[0]);
        raw.push(&[0]);
    }

    if signature.is_none() {
        return rlp_encode(&raw);
    }

    let sig = split_signature(signature.unwrap());

    let mut v = 27 + sig.recovery_param;
    if chain_id != 0 {
        raw.pop();
        raw.pop();
        raw.pop();
        v += chain_id * 2 + 8;

        if sig.v > 28 && sig.v != v {
            // TODO: throw argument error here
        }
    }

    raw.push(&v.to_le_bytes());
    raw.push(&sig.r);
    raw.push(&sig.s);

    return rlp_encode(&raw);
}
