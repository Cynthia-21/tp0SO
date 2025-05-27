package ar.edu.utn.frba.dds.fuentedinamica.models.repositories;

import ar.edu.utn.frba.dds.fuentedinamica.models.domain.Hecho;
import org.springframework.stereotype.Repository;

import java.util.List;
import java.util.Optional;

public interface IHechoRepository {
  public Hecho findById(Long id);
  public Hecho save(Hecho unhecho);
  public void delete(Hecho unHecho);
  public List<Hecho> findAll();
}
