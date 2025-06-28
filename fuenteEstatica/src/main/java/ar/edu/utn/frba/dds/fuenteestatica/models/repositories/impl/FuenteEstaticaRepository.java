package ar.edu.utn.frba.dds.fuenteestatica.models.repositories.impl;

import ar.edu.utn.frba.dds.fuenteestatica.models.domain.fuentes.Fuente;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.fuentes.FuenteEstatica;
import ar.edu.utn.frba.dds.fuenteestatica.models.repositories.IFuenteEstaticaRepository;
import org.springframework.stereotype.Repository;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

@Repository
public class FuenteEstaticaRepository implements IFuenteEstaticaRepository {

  private final List<FuenteEstatica> fuenteEstaticas = new ArrayList<>();


  @Override
  public FuenteEstatica save(FuenteEstatica fuenteEstatica) {
    fuenteEstatica.setId((long) this.fuenteEstaticas.size());
    this.fuenteEstaticas.add(fuenteEstatica);

    return fuenteEstatica;
  }

  @Override
  public List<FuenteEstatica> findAll() {
    return this.fuenteEstaticas;
  }

  @Override
  public Fuente findById(Long id) {
    return this.fuenteEstaticas.stream().filter(unaFuente -> unaFuente.getId() == id).findFirst().orElse(null);
  }

  @Override
  public void delete(FuenteEstatica fuenteEstatica) {
    this.fuenteEstaticas.remove(fuenteEstatica);
  }
}
