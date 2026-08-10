#[cxx::bridge(namespace = "notation")]
mod bridge {
    unsafe extern "C++" {
        include!("bridge.hpp");

        type Score;
        fn make_score(title: String, author: String) -> UniquePtr<Score>;
        fn score_title(score: &Score) -> String;
        fn score_author(score: &Score) -> String;
    }
}

pub use bridge::{make_score, score_author, score_title};

#[cfg(test)]
mod tests {
    use super::{make_score, score_author, score_title};

    #[test]
    fn make_score_roundtrips_title_and_author() {
        let score = make_score("My Title".to_string(), "My Author".to_string());
        assert_eq!(score_title(&score), "My Title");
        assert_eq!(score_author(&score), "My Author");
    }
}
