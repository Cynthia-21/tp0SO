package ar.edu.utn.frba.dds.fuentedinamica.models.repositories;

import ar.edu.utn.frba.dds.fuentedinamica.models.domain.fuentes.FuenteDinamica;
import org.springframework.stereotype.Repository;

import java.util.List;
import java.util.Optional;

public interface IFuenteDinamicaRepository {

  FuenteDinamica save(FuenteDinamica fuenteDinamica);
  List<FuenteDinamica> findAll();
  FuenteDinamica findById(Long id);
  void delete(FuenteDinamica fuenteDinamica);
}